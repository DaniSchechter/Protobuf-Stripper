#include "bridgeConnector.hpp"
#include "utils.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp> 
#include <boost/asio/placeholders.hpp>

BridgeConnector::BridgeConnector(std::shared_ptr<boost::asio::io_context> io_context)
  : io_context_(io_context),
    client_socket_(*io_context){}

void BridgeConnector::start()
{
  // Read the first packet from the client
  client_socket_.async_read_some(
      boost::asio::buffer(client_buffer_, max_data_length),
      boost::bind(
          &BridgeConnector::handle_client_read,
          this->shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred
      )
  );
}

void BridgeConnector::handle_client_read(const boost::system::error_code& error,
                                         std::size_t bytes_transferred)
{
  if(error) { return; }

  Logger::log(
      "Client --> Proxy     Server.   [C] " + 
      boost::lexical_cast<std::string>(client_socket_.remote_endpoint()) + 
      "  [Prev S] First read from this client" ,
      Logger::LOG_LEVEL::INFO
  );
  Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);
  
  // Resolve the remote host (If appeared in the message)
  std::string domain = Utils::parse_domain(
      boost::lexical_cast<std::string>(client_buffer_)
  );

  // If no Domain in the first message, there is nothing to do with it (we do not pereserve queue)
  if (domain == EMPTY_DOMAIN) 
  {
    Logger::log("Could not parse the domain out of the request", Logger::LOG_LEVEL::WARNING); 
    return; 
  }

  // resolve the endpoint for the requested domain
  endpoint_type endpoint  = Utils::resolve_endpoint(
      domain, *io_context_
  );

  // Could not resolve the correct endpoint fir the domain
  if(endpoint.address().to_string() == ENDPOINT_ADDRESS_ERROR) 
  {
    Logger::log("Could not resolve the domain to an endpoint", Logger::LOG_LEVEL::WARNING); 
    return; 
  }

  // Initialize the correct bridge to handle the request
  switch(endpoint.port())
  {
    case HTTP:
    {
      std::shared_ptr<HttpBridge> bridge = std::make_shared<HttpBridge>(io_context_, client_socket_);
      bridge->start_by_connect(client_buffer_, error, bytes_transferred, endpoint, domain);
      break;
    }
    case HTTPS:
    {
      std::shared_ptr<HttpsBridge> bridge = std::make_shared<HttpsBridge>(io_context_, client_socket_);
      bridge->start_by_connect(client_buffer_, error, bytes_transferred, endpoint, domain);
      break;
    }
    case SMTP:
      break;

  }
}