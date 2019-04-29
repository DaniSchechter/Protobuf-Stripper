#include "httpsBridge.hpp"

HttpsBridge::HttpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                         HttpSocketType& client_socket,
                         std::shared_ptr< boost::asio::ssl::context> context)
  : Bridge(io_context),
    client_ctx_(context),
    server_ctx_(std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23)),
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
    auto self(shared_from_this());
    client_host_ = boost::lexical_cast<std::string>( ssl_stream_->lowest_layer().remote_endpoint());

    // TODO add error message
    if(error)
    {
        return;
    }

    std::string str = "HTTP/1.1 200 connection established\r\n";
    std::copy(str.begin(), str.end() +1, server_buffer_);

      
    // HttpSocketType *s = new HttpSocketType(*io_context_); 
    // s->connect(client_socket_->lowest_layer().remote_endpoint());



    // Preform the handshake with the client
    do_handshake(client_socket_, boost::asio::ssl::stream_base::server);    

    // client_socket_->async_write_some(
    //     boost::asio::buffer(server_buffer_, strlen(server_buffer_)),
        
    //     [&, self](const boost::system::error_code& ec, std::size_t bytes_transferred)
    //     {
    //         if(ec)
    //         {
    //             Logger::log("Got error while sending 'connection established' to client: " + ec.message(), Logger::LOG_LEVEL::FATAL);
    //             return;
    //         }
            
            // Logger::log(
            //     "1) Attempting to connect to " + domain + 
            //     " [C] " + boost::lexical_cast<std::string>(client_socket_->lowest_layer().remote_endpoint()),
            //     Logger::LOG_LEVEL::INFO
            // );

            // Preform the handshake with the client
            // do_handshake(client_socket_, boost::asio::ssl::stream_base::server); 

            // Now connect to the requested server
            std::shared_ptr<SslStreamType> new_server_socket = std::make_shared<SslStreamType>(*io_context_, *server_ctx_);
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
        // }
    // );
    // do_something();
}

// void HttpsBridge::do_client_ssl_handshake(const boost::system::error_code& error,
//                                           size_t bytes_transferred,
//                                           endpoint_type endpoint,
//                                           const std::string& domain)
// {
//     if(error)
//     {
//         Logger::log("Got error while sending 'connection established' to client: " + error.message(), Logger::LOG_LEVEL::FATAL);
//         return;
//     }
    
//     std::cout << "11111111111111\n";
//     Logger::log(
//         "1) Attempting to connect to " + domain + 
//         " [C] " + boost::lexical_cast<std::string>(client_socket_->lowest_layer().remote_endpoint()),
//         Logger::LOG_LEVEL::INFO
//     );

//     std::cout << "22222222222222222\n";
//     // Now connect to the requested server
//     std::shared_ptr<SslStreamType> new_server_socket = std::make_shared<SslStreamType>(*io_context_, ctx_);
//     server_socket_map_[boost::lexical_cast<std::string>(endpoint)] = new_server_socket;

//     new_server_socket->lowest_layer().async_connect(
//         endpoint,
//         boost::bind(
//             &Bridge::handle_server_connect,
//             shared_from_this(),
//             new_server_socket,
//             boost::asio::placeholders::error,
//             bytes_transferred,
//             boost::lexical_cast<std::string>(endpoint)
//         )
//     );
    
// }

void HttpsBridge::do_handshake(std::shared_ptr<SslStreamType>& socket,
                               boost::asio::ssl::stream_base::handshake_type handshake_type)
{
    auto self(shared_from_this());
    Logger::log("starting handshake with " + socket->lowest_layer().remote_endpoint().address().to_string(), Logger::LOG_LEVEL::INFO);
    socket->handshake(handshake_type);

    Logger::log(
        "SSL handshake complited with [C]" + 
        boost::lexical_cast<std::string>(socket->lowest_layer().remote_endpoint()), 
        Logger::LOG_LEVEL::INFO
    );

    sleep(3);
    // socket->async_handshake(
    //     handshake_type,
    //     [&](const boost::system::error_code& ec)
    //     {
    //         Logger::log("11111111", Logger::LOG_LEVEL::FATAL);
    //         if(ec)
    //         {
    //             Logger::log("Error while doing handshake: " + ec.message(), Logger::LOG_LEVEL::INFO);      
    //             sleep(10);          
    //             return;
    //         }
    //         Logger::log(
    //             "SSL handshake complited with [C]" +
    //             boost::lexical_cast<std::string>(socket->lowest_layer().remote_endpoint()), 
    //             Logger::LOG_LEVEL::INFO
    //         );
    //     }
    // );
}

BasicSocketType& HttpsBridge::get_actual_socket(SslStreamType& socket)
{
    return socket.lowest_layer();
}

std::shared_ptr<SslStreamType> HttpsBridge::create_new_server_socket() 
{
    return std::make_shared<SslStreamType>(*io_context_, *server_ctx_);
    // return std::make_shared<SslStreamType>(*io_context_, *client_ctx_);
}