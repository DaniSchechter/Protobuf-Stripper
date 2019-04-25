#include "httpBridge.hpp"

HttpBridge::HttpBridge(std::shared_ptr<boost::asio::io_context> io_context,
                       HttpSocketType& socket)
  : Bridge(io_context),
    client_socket_(std::move(socket))
    {
        set_client_socket(std::move(client_socket_));
    }