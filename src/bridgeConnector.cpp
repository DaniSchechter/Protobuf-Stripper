#include "bridgeConnector.hpp"
#include "secureContextFactory.hpp"
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
  if(error) return; 
  std::string client_remote_endpoint = boost::lexical_cast<std::string>(client_socket_.remote_endpoint());

  Logger::log(
      "Client --> Proxy     Server.   [C] " + client_remote_endpoint + 
      " [Prev S] First read from this client" ,
      Logger::LOG_LEVEL::INFO
  );
  Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);
  
  // Resolve the remote host (If it appeared in the message)
  std::string domain; 
  int parsing_error = Utils::parse_domain(
      boost::lexical_cast<std::string>(client_buffer_), domain
  );

  // If no Domain in the first message, there is nothing to do with it (we do not pereserve queue)
  if (parsing_error == Utils::EMPTY_DOMAIN) 
  {
    Logger::log("Could not parse the domain out of the request", Logger::LOG_LEVEL::WARNING); 
    return; 
  }

  // resolve the endpoint for the requested domain
  endpoint_type endpoint  = Utils::resolve_endpoint(
      domain, *io_context_
  );

  std::string server_host = boost::lexical_cast<std::string>(endpoint);

  // Could not resolve the correct endpoint for the domain
  if( server_host == ENDPOINT_ADDRESS_ERROR) 
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
      handle_connect_request(client_remote_endpoint);

      // Generate the context for the ssl
      std::unique_ptr<boost::asio::ssl::context> ctx = SecureContextFactory::create_context(domain);
      if(!ctx) return;

      std::shared_ptr<HttpsBridge> bridge = std::make_shared<HttpsBridge>(
        io_context_, client_socket_, std::move(*ctx)
      );

      bridge->start_by_connect(client_buffer_, error, bytes_transferred, endpoint, domain);
      break;
    }
    default:

      // Send confirmation about tunnel creation, if needed
      if(strstr(client_buffer_, "CONNECT"))
      {
        handle_connect_request(client_remote_endpoint);
      }

      // Check if a random port is associated with ftp connection channel 
      if(FtpsBridge::check_if_session_is_cached(endpoint.port()))
      {
        // Instantiate a new secure FTPS bridge
        std::unique_ptr<boost::asio::ssl::context> ctx = SecureContextFactory::create_context(domain);
        if(!ctx) return;

        std::shared_ptr<FtpsBridge> bridge = std::make_shared<FtpsBridge>(
          io_context_, std::make_shared<HttpSocketType>(std::move(client_socket_)), endpoint, std::move(*ctx), server_host
        );

        bridge->run(server_host);
        return;
      }
      else
      {
        std::shared_ptr<FtpBridge> bridge = std::make_shared<FtpBridge>(io_context_, client_socket_, domain);
        bridge->start_by_connect(client_buffer_, error, bytes_transferred, endpoint, domain);
      }

  }
}

void BridgeConnector::handle_connect_request(const std::string& client_remote_endpoint)
{
  std::string str = "HTTP/1.1 200 connection established\r\n\r\n";
  client_socket_.write_some(boost::asio::buffer(str, str.length()));

  Logger::log(
    "Client <-- Proxy     Server.   [C] " + client_remote_endpoint + "\n" + str,
    Logger::LOG_LEVEL::INFO
  );
}