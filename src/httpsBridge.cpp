#include "httpsBridge.hpp"

HttpsBridge::HttpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                         HttpSocketType& socket)
  : Bridge(io_context),
    ctx_(boost::asio::ssl::context::sslv23),
    ssl_stream_(std::move(socket), ctx_)
{ 
    // ssl_
}
