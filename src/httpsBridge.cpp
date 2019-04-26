#include "httpsBridge.hpp"

HttpsBridge::HttpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                         HttpSocketType& client_socket)
  : Bridge(io_context),
    ctx_(boost::asio::ssl::context::sslv23),
    ssl_stream_(std::make_shared<SslStreamType>(std::move(client_socket), ctx_))
{ 
    set_client_socket(ssl_stream_);
    client_host_ = boost::lexical_cast<std::string>( ssl_stream_->lowest_layer().remote_endpoint());
}

void HttpsBridge::start_by_connect(char client_buffer [max_data_length],
                                    const boost::system::error_code& error,
                                    std::size_t bytes_transferred,
                                    endpoint_type endpoint,
                                    const std::string& domain)
{
    // TODO add error message
    if(error)
    {
        return;
    }

    std::string str = "HTTP/1.1 200 connection established\r\n";
    std::copy(str.begin(), str.end() +1, client_buffer_);

    std::cout << "the client buffer is: " << client_buffer_ << std::endl;

        std::cout << client_socket_->lowest_layer().remote_endpoint() << "\n";


    async_write(
        *client_socket_,
        boost::asio::buffer(client_buffer_, strlen(client_buffer_)),
        [this, endpoint, domain](boost::system::error_code& error, std::size_t bytes_transferred)
        {
            if(error)
            {
                // Logger::log()
                return;
            }
            // Preform the handshake with the client
            do_handshake(client_socket_, boost::asio::ssl::stream_base::server);

            Logger::log(
                "1) Attempting to connect to " + domain + 
                " [C] " + boost::lexical_cast<std::string>(client_socket_->lowest_layer().remote_endpoint()),
                Logger::LOG_LEVEL::INFO
            );

            // Now connect to the requested server
            std::shared_ptr<SslStreamType> new_server_socket = std::make_shared<SslStreamType>(*io_context_, ctx_);
            server_socket_map_[boost::lexical_cast<std::string>(endpoint)] = new_server_socket;

            new_server_socket->lowest_layer().async_connect(
                endpoint,
                boost::bind(
                    &Bridge::handle_server_connect,
                    shared_from_this(),
                    new_server_socket,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred,
                    boost::lexical_cast<std::string>(endpoint)
                )
            ); 
        }
    );
}

// void HttpsBridge::do_client_ssl_handshake(const boost::system::error_code& error,
//                                           size_t bytes_transferred,
//                                           endpoint_type endpoint,
//                                           const std::string& domain)
// {
//     if(error)
//     {
//         Logger::log(
//             "Error returning to client 'connection established' message",
//             Logger::LOG_LEVEL::WARNING
//         );
//         return;
//     }

    
// }

void HttpsBridge::do_handshake(std::shared_ptr<SslStreamType> socket,
                               boost::asio::ssl::stream_base::handshake_type handshake_type)
{
    std::cout << "starting hanshake\n";
    socket->handshake(
        handshake_type
    );
    Logger::log(
        "SSL handshake complited with [C]" + 
        boost::lexical_cast<std::string>(socket->lowest_layer().remote_endpoint()), 
        Logger::LOG_LEVEL::INFO
    );
}

BasicSocketType& HttpsBridge::get_actual_socket(SslStreamType& socket)
{
    return socket.lowest_layer();
}

std::shared_ptr<SslStreamType> HttpsBridge::create_new_server_socket() 
{
    return std::make_shared<SslStreamType>(*io_context_, ctx_);
}