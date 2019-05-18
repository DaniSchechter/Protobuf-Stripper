#define LEAF_CERTIFICATE_SCRIPT "./scripts/generate_certificate.sh ./keys "

#include "bridgeConnector.hpp"
#include "utils.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp> 
#include <boost/asio/placeholders.hpp> 

std::unordered_set< std::string > BridgeConnector::host_certificate_set_;
std::mutex BridgeConnector::certificate_map_lock_;

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
  std::string client_remote_endpoint = boost::lexical_cast<std::string>(client_socket_.remote_endpoint());

  Logger::log(
      "Client --> Proxy     Server.   [C] " + client_remote_endpoint + 
      " [Prev S] First read from this client" ,
      Logger::LOG_LEVEL::INFO
  );

  Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);
  
  // Resolve the remote host (If appeared in the message)
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
      std::string str = "HTTP/1.1 200 connection established\r\n\r\n";
      client_socket_.write_some(boost::asio::buffer(str, str.length()));

      Logger::log(
        "Client <-- Proxy     Server.   [C] " + client_remote_endpoint + "\n" + str,
        Logger::LOG_LEVEL::INFO
      );

      std::string common_name;
      if( Utils::split_domain(domain, common_name) == Utils::COMMON_NAME_ERROR )
      {
        Logger::log(
          "No match while parsing common name out of the domain: " + domain, Logger::LOG_LEVEL::FATAL
        );
        return;
      }

      // If this is a new domain
      BridgeConnector::certificate_map_lock_.lock();
      if (host_certificate_set_.find(common_name) == host_certificate_set_.end())
      {
        Logger::log(
            "Generating a new certificate for\nFull domain: " + domain +
            "\nCommon name: " + common_name, Logger::LOG_LEVEL::INFO
        );
        // Set the new certificate file for the requested domain
        std::string script_command = boost::lexical_cast<std::string>(LEAF_CERTIFICATE_SCRIPT) + common_name;
        if(system(script_command.c_str()) != 0)
        {
          Logger::log(
            "Error generating a new certificate for" + common_name, Logger::LOG_LEVEL::FATAL
          );
          return;
        }
        BridgeConnector::host_certificate_set_.insert(common_name);
      }      
      BridgeConnector::certificate_map_lock_.unlock();

      // Initialize the context

      std::shared_ptr<boost::asio::ssl::context> ctx = std::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
      ctx->set_options(
        boost::asio::ssl::context::default_workarounds
        | boost::asio::ssl::context::no_sslv2
        | boost::asio::ssl::context::no_sslv3);
      ctx->set_password_callback(boost::bind(&BridgeConnector::get_password, this));

      ctx->use_certificate_chain_file("keys/" + common_name + "/" + common_name + ".crt");
      ctx->use_private_key_file("keys/" + common_name + "/" + common_name + ".key", boost::asio::ssl::context::pem);

      std::shared_ptr<HttpsBridge> bridge = std::make_shared<HttpsBridge>(io_context_, client_socket_, ctx);
      bridge->start_by_connect(client_buffer_, error, bytes_transferred, endpoint, domain);
      break;
    }
    case FTP:
    {
      std::shared_ptr<FtpBridge> bridge = std::make_shared<FtpBridge>(io_context_, client_socket_);
      bridge->start_by_connect(client_buffer_, error, bytes_transferred, endpoint, domain);
      break;
    }
    default:
       Logger::log(
        "No suppoort for protocol on port " + endpoint.port(), Logger::LOG_LEVEL::WARNING
      );
  }
}