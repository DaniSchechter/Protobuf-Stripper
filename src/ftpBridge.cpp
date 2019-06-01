#include "ftpBridge.hpp"
#include "utils.hpp"
#include "secureContextFactory.hpp"
#include <boost/lexical_cast.hpp>
#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <regex>


FtpBridge::FtpBridge(std::shared_ptr<boost::asio::io_context> io_context,
                     HttpSocketType& client_socket,
                     const std::string& domain)
  : Bridge(io_context)
{
    set_client_socket(std::make_shared<HttpSocketType>(std::move(client_socket)));
    domain_ = domain;
}

void FtpBridge::handle_server_read(std::shared_ptr<HttpSocketType> server_socket,
                                   const boost::system::error_code& error,
                                   const size_t& bytes_transferred,
                                   const std::string& server_host)
{
    if(error) {
        strand_.post(
            boost::bind(
                &Bridge::close,
                this->shared_from_this(),
                server_socket, 
                Bridge::SOCKET_ERROR_SOURCE::SERVER_READ_ERROR, 
                server_host, 
                error
            )
        );
        return;
    } 
    
    Logger::log(
        "Client     Proxy <-- Server.   [C] " + 
        client_host_ + "  [S] " + server_host,
        Logger::LOG_LEVEL::INFO
    );
    Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::DEBUG);

    // Upgrading to ftp secure, If indicated in the CONTROL channel ("Proceed with negotiation")
    if(strstr(server_buffer_, SECURE_UPGRADE_MESSAGE))
    {   
        // Forward the acknowledgement message for secure connection to the client
        write(
            *client_socket_,
            boost::asio::buffer(server_buffer_,bytes_transferred)
        );
        
        // Stop all async operations on the unsecure socket, as a preperation for secure ssl stream wrapper
        client_socket_->cancel();

        Logger::log(
            "Client <-- Proxy     Server.   [C] " + 
            client_host_ + "  [S] " + server_host,
            Logger::LOG_LEVEL::INFO
        );
        Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::DEBUG);

        // Generate the context for the new secure connection
        std::unique_ptr<boost::asio::ssl::context> ctx = SecureContextFactory::create_context(domain_);
        if(!ctx) return;

        // Instantiate a new secure FTPS bridge ( Connection inatruction channel)
        std::shared_ptr<FtpsBridge> bridge = std::make_shared<FtpsBridge>(io_context_, client_socket_, server_socket ,std::move(*ctx), server_host);
        bridge->run(server_host);
        
        // Close the current bridge - the communication will be handled by FTPS bridge that have been just created
        strand_.post(
            boost::bind(
                &Bridge::close,
                this->shared_from_this(),
                server_socket, 
                Bridge::SOCKET_ERROR_SOURCE::BRIDGE_UPGRADED, 
                server_host, 
                error
            )
        );
        return;
    }
    else 
    {
        async_write(
            *client_socket_,
            boost::asio::buffer(server_buffer_,bytes_transferred),
            boost::bind(
                &Bridge::handle_client_write,
                this->shared_from_this(),
                server_socket,
                boost::asio::placeholders::error,
                server_host
            )
        );
    }

}

std::shared_ptr<HttpSocketType> FtpBridge::create_new_server_socket()
{
  return std::make_shared<HttpSocketType>(*io_context_);
}
