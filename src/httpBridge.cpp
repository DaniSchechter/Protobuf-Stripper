#include "httpBridge.hpp"

HttpBridge::HttpBridge(std::shared_ptr<boost::asio::io_context> io_context,
                       HttpSocketType& client_socket)
  : Bridge(io_context)
{
    set_client_socket(std::make_shared<HttpSocketType>(std::move(client_socket)));
}

std::shared_ptr<HttpSocketType> HttpBridge::create_new_server_socket()
{
  return std::make_shared<HttpSocketType>(*io_context_);
}

void HttpBridge::save_first_message(char client_buffer [max_data_length])
{
    strncpy(client_buffer_, client_buffer, max_data_length);
}