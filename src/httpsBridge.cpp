#include "httpsBridge.hpp"
#include <boost/lexical_cast.hpp>


HttpsBridge::HttpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                         HttpSocketType& client_socket,
                         boost::asio::ssl::context context)
  : Bridge(io_context),
    ctx_(std::move(context))
{ 
    set_client_socket(std::make_shared<SslStreamType>(std::move(client_socket), ctx_));
}

void HttpsBridge::do_handshake(std::shared_ptr<SslStreamType> socket,
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

std::shared_ptr<SslStreamType> HttpsBridge::create_new_server_socket() 
{
    return std::make_shared<SslStreamType>(*io_context_, ctx_);
}

void HttpsBridge::close_socket(std::shared_ptr<SslStreamType> socket)
{
    boost::system::error_code ignored_ec;
    socket->shutdown(ignored_ec);
    socket->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    socket.reset();
}