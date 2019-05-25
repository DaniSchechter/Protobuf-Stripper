#include "ftpsBridge.hpp"

FtpsBridge::FtpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                       HttpSocketType& client_socket,
                       std::shared_ptr< boost::asio::ssl::context> context)
    : ctx_(context),
      client_socket_(std::make_shared<HttpSocketType>(std::move(client_socket)))
{
}

void FtpsBridge::start_by_connect(char client_buffer [max_data_length],
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
    std::cout << "0000000000000000\n";
    strncpy(client_buffer, "220 Welcome to Idodo FTP service.", strlen("220 Welcome to Idodo FTP service."));

    client_socket_->write_some(
        boost::asio::buffer(client_buffer, bytes_transferred)
    );
    
    std::cout << "111111111111111111\n";
    client_socket_->read_some(
        boost::asio::buffer(client_buffer, bytes_transferred)
    );
    std::cout << "2222222222222222222222\n";

    if(!strstr(client_buffer, "AUTH TLS"))
    {
        Logger::log("&&&&&&&&&&&&&&&&&&&&&&&&&&", Logger::LOG_LEVEL::FATAL);
    }
    std::cout << "333333333333333333333\n";

    ssl_stream_ = std::make_shared<SslStreamType>(std::move(*client_socket_), *ctx_);

    Logger::log(
        "SSL handshake started",
        Logger::LOG_LEVEL::INFO
    );
    std::cout << "4444444444444444444444\n";

    ssl_stream_->handshake(boost::asio::ssl::stream_base::server);

    Logger::log(
        "SSL handshake completed",
        Logger::LOG_LEVEL::INFO
    );
}



void FtpsBridge::do_handshake(std::shared_ptr<SslStreamType>& socket,
                               boost::asio::ssl::stream_base::handshake_type handshake_type)
{
    // std::string endpoint = boost::lexical_cast<std::string>(socket->lowest_layer().remote_endpoint());
    // Logger::log(
    //     "SSL handshake started with " + endpoint + " [C] " + client_host_,
    //     Logger::LOG_LEVEL::INFO
    // );

    // socket->handshake(handshake_type);

    // Logger::log(
    //     "SSL handshake completed with " + endpoint,
    //     Logger::LOG_LEVEL::INFO
    // );
}

BasicSocketType& FtpsBridge::get_actual_socket(SslStreamType& socket)
{
    return socket.lowest_layer();
}
