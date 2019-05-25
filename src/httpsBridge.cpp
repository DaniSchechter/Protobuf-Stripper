#include "httpsBridge.hpp"

HttpsBridge::HttpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                         HttpSocketType& client_socket,
                         std::shared_ptr< boost::asio::ssl::context> context)
  : Bridge(io_context),
    ctx_(context),
    ssl_stream_(std::make_shared<SslStreamType>(std::move(client_socket), *context))
{ 
    set_client_socket(ssl_stream_);
}

void HttpsBridge::start_by_connect(char client_buffer [max_data_length],
                                   const boost::system::error_code& error,
                                   std::size_t bytes_transferred,
                                   endpoint_type endpoint,
                                   const std::string& domain)
{
    client_host_ = boost::lexical_cast<std::string>( ssl_stream_->lowest_layer().remote_endpoint());

    // TODO add error message
    if(error)
    {
        return;
    }

    // Preform the handshake with the client
    do_handshake(client_socket_, boost::asio::ssl::stream_base::server);    

    // Create a new server socket 
    std::shared_ptr<SslStreamType> new_server_socket = std::make_shared<SslStreamType>(*io_context_, *ctx_);
    server_socket_map_[boost::lexical_cast<std::string>(endpoint)] = new_server_socket;
    
    new_server_socket->lowest_layer().async_connect(
        endpoint,
        boost::bind(
            &Bridge::handle_server_connect,
            shared_from_this(),
            new_server_socket,
            boost::asio::placeholders::error,
            bytes_transferred,
            boost::lexical_cast<std::string>(endpoint)
        )
    ); 
}



void HttpsBridge::do_handshake(std::shared_ptr<SslStreamType>& socket,
                               boost::asio::ssl::stream_base::handshake_type handshake_type)
{
    std::string endpoint = boost::lexical_cast<std::string>(socket->lowest_layer().remote_endpoint());
    Logger::log(
        "SSL handshake started with " + endpoint + " [C] " + client_host_,
        Logger::LOG_LEVEL::INFO
    );

    socket->handshake(handshake_type);

    Logger::log(
        "SSL handshake completed with " + endpoint,
        Logger::LOG_LEVEL::INFO
    );
}

BasicSocketType& HttpsBridge::get_actual_socket(SslStreamType& socket)
{
    return socket.lowest_layer();
}

std::shared_ptr<SslStreamType> HttpsBridge::create_new_server_socket() 
{
    return std::make_shared<SslStreamType>(*io_context_, *ctx_);
}