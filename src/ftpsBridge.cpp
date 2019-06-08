#include "ftpsBridge.hpp"

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <regex>

std::unordered_map<std::string, SSL_SESSION*> FtpsBridge::session_cache_;
std::mutex FtpsBridge::session_cache_lock_;


FtpsBridge::FtpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                       std::shared_ptr<HttpSocketType> unsecure_client_socket,
                       std::shared_ptr<HttpSocketType> unsecure_server_socket,
                       boost::asio::ssl::context context,
                       const std::string& unsecure_server_host)
    : Bridge(io_context),
      ctx_(std::move(context))
{
    // Wrap the unsecure client socket with a secure ssl stream
    set_client_socket(std::make_shared<SslStreamType>(std::move(*unsecure_client_socket), ctx_));

    // Wrap the unsecure server socket with a secure ssl stream
    secure_server_socket_ = std::make_shared<SslStreamType>(std::move(*unsecure_server_socket), ctx_);

    // Save the socket into bridge's server sockets map
    server_socket_map_[unsecure_server_host] = secure_server_socket_;

    client_host_ = boost::lexical_cast<std::string>(client_socket_->lowest_layer().remote_endpoint());
}

FtpsBridge::FtpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                       std::shared_ptr<HttpSocketType> unsecure_client_socket,
                       endpoint_type endpoint,
                       boost::asio::ssl::context context,
                       const std::string& unsecure_server_host)
    : Bridge(io_context),
      ctx_(std::move(context))
{
    const std::string server_port_to_connect = std::to_string(endpoint.port());

    // Wrap the unsecure client socket with a secure ssl stream
    set_client_socket(std::make_shared<SslStreamType>(std::move(*unsecure_client_socket), ctx_));

    // Reuse previous session
    auto reusable_session = FtpsBridge::session_cache_.find(server_port_to_connect);
    if(reusable_session == FtpsBridge::session_cache_.end())
    {
        Logger::log("Could not resuse SSL_SESSION, on port: " + server_port_to_connect, Logger::LOG_LEVEL::FATAL);
    }

    // Create a secure server socket and resuse the previous session
    secure_server_socket_ = std::make_shared<SslStreamType>(*io_context_, ctx_);
    SSL_set_session(secure_server_socket_->native_handle(), reusable_session->second);

    secure_server_socket_->lowest_layer().connect(endpoint); 

    // Save the socket into bridge's server sockets map
    server_socket_map_[unsecure_server_host] = secure_server_socket_;

    client_host_ = boost::lexical_cast<std::string>(client_socket_->lowest_layer().remote_endpoint());
}

void FtpsBridge::handle_server_read(std::shared_ptr<SslStreamType> server_socket,
                                    const boost::system::error_code& error,
                                    const size_t& bytes_transferred,
                                    const std::string& server_host)
{
    if(error) {
        strand_.post(
            boost::bind(
                &Bridge::close,
                this->shared_from_this(),
                server_socket, 
                Bridge::SOCKET_ERROR_SOURCE::SERVER_READ_ERROR, 
                server_host, 
                error
            )
        );
        return;
    } 
    
    Logger::log(
        "Client     Proxy <-- Server.   [C] " + 
        client_host_ + "  [S] " + server_host,
        Logger::LOG_LEVEL::INFO
    );
    Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::DEBUG);

    // Check if the message contains the port forwarding for the data connection 
    if(strstr(server_buffer_, DATA_CONNECTION_PORT_MESSAGE))
    {
        std::string port;
        if(!FtpsBridge::fetch_data_channel_ports(server_buffer_, port)) 
            return;

        FtpsBridge::session_cache_lock_.lock();
        FtpsBridge::session_cache_[port] = SSL_get1_session(server_socket->native_handle());
        FtpsBridge::session_cache_lock_.unlock();
    }

    async_write(
        *client_socket_,
        boost::asio::buffer(server_buffer_,bytes_transferred),
        boost::bind(
            &Bridge::handle_client_write,
            this->shared_from_this(),
            server_socket,
            boost::asio::placeholders::error,
            server_host
        )
    );
}

void FtpsBridge::run(const std::string& unsecure_server_host)
{
    // Perform handshake with the client
    Logger::log(
        "SSL handshake started with the client [C] " + client_host_,
        Logger::LOG_LEVEL::INFO
    );

    client_socket_->handshake(boost::asio::ssl::stream_base::server);

    Logger::log(
        "SSL handshake completed with the client [C] " + client_host_,
        Logger::LOG_LEVEL::INFO
    );

    // Perform handshake with the server    
    Logger::log(
        "SSL handshake started with server [C] " + client_host_ + " ( Wont be handshake completed )",
        Logger::LOG_LEVEL::INFO
    );
    
    secure_server_socket_->async_handshake(
        boost::asio::ssl::stream_base::client,
        boost::bind(
           &Bridge::handle_server_connect,
           this->shared_from_this(),
           secure_server_socket_,
           boost::asio::placeholders::error,
           unsecure_server_host
        )
    );
}

std::shared_ptr<SslStreamType> FtpsBridge::create_new_server_socket() 
{
    return std::make_shared<SslStreamType>(*io_context_, ctx_);
}

bool FtpsBridge::fetch_data_channel_ports(const std::string& message, std::string& port)
{
    std::regex re(".*?[|]{3}(?:(\\d+)[|]?).*");
    std::smatch match;
    std::regex_search(message, match, re);

    // If there is no valid URI
    if(!match[1].matched)
    {
        return false;
    }
    port.assign(match[1].str(), 0, match[1].str().length());
    return true;
}

bool FtpsBridge::check_if_session_is_cached(const unsigned short port)
{
    FtpsBridge::session_cache_lock_.lock();
    bool exist = FtpsBridge::session_cache_.find(std::to_string(port)) != FtpsBridge::session_cache_.end();
    FtpsBridge::session_cache_lock_.unlock();
    return exist;
}

void FtpsBridge::close_socket(std::shared_ptr<SslStreamType> socket)
{
    boost::system::error_code ignored_ec;
    socket->shutdown(ignored_ec);
    socket->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    socket.reset();
}