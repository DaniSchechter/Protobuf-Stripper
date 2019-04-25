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

BasicSocketType& HttpsBridge::get_actual_socket(SslStreamType& socket)
{
    return socket.lowest_layer();
}

std::shared_ptr<SslStreamType> HttpsBridge::create_new_server_socket() 
{
    return std::make_shared<SslStreamType>(*io_context_, ctx_);
}