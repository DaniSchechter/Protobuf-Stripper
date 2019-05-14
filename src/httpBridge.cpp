#include "httpBridge.hpp"

HttpBridge::HttpBridge(std::shared_ptr<boost::asio::io_context> io_context,
                       HttpSocketType& client_socket)
  : Bridge(io_context)
{
    set_client_socket(std::make_shared<HttpSocketType>(std::move(client_socket)));
    client_host_ = boost::lexical_cast<std::string>( client_socket_->remote_endpoint());
}

void HttpBridge::start_by_connect(char client_buffer [max_data_length],
                                  const boost::system::error_code& error,
                                  std::size_t bytes_transferred,
                                  endpoint_type endpoint,
                                  const std::string& domain)
{
    // save the message  
    strncpy(client_buffer_, client_buffer, max_data_length);
    Logger::log(
        "First domain for this Bridge\nAttempting to connect to " + domain + 
        " [C] " + boost::lexical_cast<std::string>(client_socket_->lowest_layer().remote_endpoint()),
        Logger::LOG_LEVEL::INFO
    );

    // Create a new server socket and insert to the map for future reuse
    std::shared_ptr<HttpSocketType> new_server_socket = std::make_shared<HttpSocketType>(*io_context_);
    server_socket_map_[boost::lexical_cast<std::string>(endpoint)] = new_server_socket;

    new_server_socket->lowest_layer().async_connect(
        endpoint,
        boost::bind(
            &Bridge::handle_server_connect,
            this->shared_from_this(),
            new_server_socket,
            error,
            bytes_transferred,
            boost::lexical_cast<std::string>(endpoint),
            false
        )
    ); 
}

void HttpBridge::do_handshake(std::shared_ptr<HttpSocketType> socket,
                               boost::asio::ssl::stream_base::handshake_type handshake_type)
{
    return;
}

HttpSocketType& HttpBridge::get_actual_socket(HttpSocketType& socket)
{
  return socket;
}

std::shared_ptr<HttpSocketType> HttpBridge::create_new_server_socket()
{
  return std::make_shared<HttpSocketType>(*io_context_);
}