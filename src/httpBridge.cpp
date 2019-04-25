#include "httpBridge.hpp"

HttpBridge::HttpBridge(std::shared_ptr<boost::asio::io_context> io_context,
                       HttpSocketType& client_socket)
  : Bridge(io_context)
{
    set_client_socket(std::make_shared<HttpSocketType>(client_socket));
}