#include "httpsBridge.hpp"

HttpsBridge::HttpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                         HttpSocketType& client_socket)
  : Bridge(io_context),
    ctx_(boost::asio::ssl::context::sslv23),
    ssl_stream_(std::make_shared<SslStreamType>(std::move(client_socket), ctx_))
{ 
    set_client_socket(ssl_stream_);
}
