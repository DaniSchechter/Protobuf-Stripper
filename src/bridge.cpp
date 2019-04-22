#include "bridge.hpp"

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/write.hpp>
#include <utility>

Bridge::Bridge(std::shared_ptr<boost::asio::io_context> io_context)
  : strand_(*io_context),
    io_context_(io_context),
    client_socket_(*io_context){}

socket_type& Bridge::client_socket()
{
  return client_socket_;
}

void Bridge::start()
{
    // Read the first packet from the client and resolve its destination
    client_socket_.async_read_some(
        boost::asio::buffer(client_buffer_, max_data_length),
        boost::bind(
            &Bridge::handle_client_read,
            shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

void Bridge::handle_server_connect(std::shared_ptr<socket_type> server_socket,
                                   const boost::system::error_code& error,
                                   std::size_t bytes_transferred,
                                   const std::string& endpoint)
{
    if(error)
    {
        // TODO think of edge cases
        close(server_socket, Bridge::SOCKET_ERROR_SOURCE::SERVER_CONNECT_ERROR, endpoint);
        return;
    }
    Logger::log(
        "Connection established.   [C] " + 
        client_host_ + "  [S] " + endpoint,
        Logger::LOG_LEVEL::WARNING
    );    
    // Setup async read from remote server (server). Preperation for the following write
    server_socket->async_read_some(
        boost::asio::buffer(server_buffer_,max_data_length),
        boost::bind(
            &Bridge::handle_server_read,
            shared_from_this(),
            server_socket,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred,
            endpoint
        )
    );

    // Send the first messaage to the server
    async_write(
        *server_socket,
        boost::asio::buffer(client_buffer_,bytes_transferred),
        boost::bind(
            &Bridge::handle_server_write,
            shared_from_this(),
            server_socket,
            boost::asio::placeholders::error,
            endpoint
        )
    );
}
void Bridge::handle_client_read(const boost::system::error_code& error,
                                std::size_t bytes_transferred)
{
    if(error) { return; }
    client_host_ = boost::lexical_cast<std::string>(client_socket_.remote_endpoint());
    Logger::log(
        "Client --> Proxy     Server.   [C] " + 
        client_host_ + "  [Prev S] First read from this client" ,
        Logger::LOG_LEVEL::WARNING
    );
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);
    
    // Resolve the remote host (If appeared in the message)
    std::string domain = Utils::parse_domain(
        boost::lexical_cast<std::string>(client_buffer_)
    );

    // If no Domain in the first message, there is nothing to do with it
    if (domain == EMPTY_DOMAIN) { return; }

    endpoint_type endpoint  = Utils::resolve_endpoint(
        domain, io_context_
    );

    if(endpoint.address().to_string() == ENDPOINT_ADDRESS_ERROR) { return; }
    Logger::log(
        "1) Attempting to connect to " + domain + 
        " [C] " + boost::lexical_cast<std::string>(client_socket_.remote_endpoint()),
        Logger::LOG_LEVEL::INFO
    );
    // Create a new server socket and insert to the map for future reuse
    std::shared_ptr<socket_type> new_server_socket = std::make_shared<socket_type>((*io_context_));
    server_socket_map_[domain] = new_server_socket;

    new_server_socket->async_connect(
        endpoint,
        boost::bind(
            &Bridge::handle_server_connect,
            shared_from_this(),
            new_server_socket,
            error,
            bytes_transferred,
            boost::lexical_cast<std::string>(endpoint)
        )
    );
}

// Reading from client is completed, now send the data to the remote server
void Bridge::handle_client_read(std::shared_ptr<socket_type> server_socket, 
                                const boost::system::error_code& error,
                                std::size_t bytes_transferred,
                                const std::string& endpoint)
{
    if(error)
    {
            close(server_socket, Bridge::SOCKET_ERROR_SOURCE::CLIENT_READ_ERROR, endpoint);
            return;
    }
    Logger::log(
        "Client --> Proxy     Server.   [C] " + 
        client_host_ + "  [S] " + endpoint,
        Logger::LOG_LEVEL::WARNING
    );
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);
    
    // Resolve the remote host (If appeared in the message)
    std::string domain = Utils::parse_domain(
        boost::lexical_cast<std::string>(client_buffer_)
    );

    // No new Host was provided in the message - use the current one
    if (domain == EMPTY_DOMAIN)
    {
        async_write(
            *server_socket,
            boost::asio::buffer(client_buffer_,bytes_transferred),
            boost::bind(
                &Bridge::handle_server_write,
                shared_from_this(),
                server_socket,
                boost::asio::placeholders::error,
                endpoint
            )
        );
    } 
    
    // New Host was written in the message - The endpoint is valid
    else
    {
        // Get the server socket associated with the remote host
        auto server_socket = server_socket_map_.find(domain);

        // Check if there is already a socket for this endpoint
        if(server_socket == server_socket_map_.end()) // doesnt exist yet
        {
            endpoint_type endpoint  = Utils::resolve_endpoint(
                domain, io_context_
            );

            if(endpoint.address().to_string() == ENDPOINT_ADDRESS_ERROR) { 
                Logger::log(
                    "Got a new domain: " + domain + ", and could not resolve it to an endpoint",
                    Logger::LOG_LEVEL::WARNING
                );

                // Read again, since we was provided with a new domain so we can't sent the message to the prev server
                //TODO call the read again
            }
            Logger::log(
                "2) Attempting to connect to " + domain + 
                " [C] " + client_host_,
                Logger::LOG_LEVEL::INFO
            );
            // Create a new socket and start using it
            std::shared_ptr<socket_type> new_server_socket = std::make_shared<socket_type>((*io_context_));
            server_socket_map_[domain] = new_server_socket;

            new_server_socket->async_connect(
                endpoint,
                boost::bind(
                    &Bridge::handle_server_connect,
                    shared_from_this(),
                    new_server_socket,
                    error,
                    bytes_transferred,
                    boost::lexical_cast<std::string>(endpoint)
                )
            );
        }

        // If there is a socket, continue in the cycle
        else
        {
            async_write(
                *(server_socket->second),
                boost::asio::buffer(client_buffer_,bytes_transferred),
                boost::bind(
                    &Bridge::handle_server_write,
                    shared_from_this(),
                    server_socket->second,
                    boost::asio::placeholders::error,
                    endpoint
                )
            );
        }
    }

}

// Write to remote server complete, Async read from client
void Bridge::handle_server_write(std::shared_ptr<socket_type> server_socket,
                                 const boost::system::error_code& error,
                                 const std::string& endpoint)
{
    if(error) 
    {
        close(server_socket, Bridge::SOCKET_ERROR_SOURCE::SERVER_WRITE_ERROR, endpoint);
        return;
    }
    Logger::log(
        "Client     Proxy --> Server.   [C] " + 
        client_host_ + "  [S] " + endpoint,
        Logger::LOG_LEVEL::WARNING
    );
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);

    client_socket_.async_read_some(
        boost::asio::buffer(client_buffer_,max_data_length),
        boost::bind(
            &Bridge::handle_client_read,
            shared_from_this(),
            server_socket,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred,
            endpoint
        )
    );
}

// Read from client complete, now send data to remote server
void Bridge::handle_server_read(std::shared_ptr<socket_type> server_socket,
                                const boost::system::error_code& error,
                                const size_t& bytes_transferred,
                                const std::string& endpoint)
{
    if(error) {
        close(server_socket, Bridge::SOCKET_ERROR_SOURCE::SERVER_READ_ERROR, endpoint);
        return;
    } 
    Logger::log(
        "Client     Proxy <-- Server.   [C] " + 
        client_host_ + "  [S] " + endpoint,
        Logger::LOG_LEVEL::WARNING
    );
    Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::DEBUG);

    async_write(
        client_socket_,
        boost::asio::buffer(server_buffer_,bytes_transferred),
        boost::bind(
            &Bridge::handle_client_write,
            shared_from_this(),
            server_socket,
            boost::asio::placeholders::error,
            endpoint
        )
    );
}

void Bridge::handle_client_write(std::shared_ptr<socket_type> server_socket,
                                 const boost::system::error_code& error,
                                 const std::string& endpoint)
{
    if(error) 
    {
        close(server_socket, Bridge::SOCKET_ERROR_SOURCE::CLIENT_READ_ERROR, endpoint); 
        return;
    }
    Logger::log(
        "Client <-- Proxy     Server.   [C] " + 
        client_host_ + "  [S] " + endpoint,
        Logger::LOG_LEVEL::WARNING
    );

    Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::DEBUG);
           
    server_socket->async_read_some(
        boost::asio::buffer(server_buffer_,max_data_length),
        boost::bind(
            &Bridge::handle_server_read,
            shared_from_this(),
            server_socket,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred,
            endpoint
        )
    );
}

void Bridge::close(std::shared_ptr<socket_type> server_socket,  
                   SOCKET_ERROR_SOURCE error_source,
                   const std::string& endpoint)
{
    //print the error origin
    Bridge::print_error_source(error_source);

    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;

    // If the error is in the clinet socket, close it as well as all server sockets
    if( error_source == Bridge::SOCKET_ERROR_SOURCE::CLIENT_WRITE_ERROR || 
        error_source == Bridge::SOCKET_ERROR_SOURCE::CLIENT_READ_ERROR )
    {
        if (client_socket_.is_open())
        {
            client_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
            client_socket_.close();
        }
        for(const auto& server_socket_iter: server_socket_map_)
        {
            server_socket_iter.second->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
            server_socket_iter.second->close();
        }
        server_socket_map_.clear();
    }

    
    // Error is with one of the server sockets in the map
    else if(server_socket->is_open())
    {
        Logger::log(
            "Deleting [S] " + endpoint + " for [C] ",
            Logger::LOG_LEVEL::DEBUG
        );
        server_socket_map_.erase(endpoint);

        // TODO if the erase raises exception we need to find the server socket and erase using the iterator
        server_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        server_socket->close();

        // if it is the last server socket for this bridge, clear the client socket and close the bridge
        if(server_socket_map_.size() == 0)
        {
            Logger::log(
                "All servers are gone, clearing up this bridge [C] " +
                client_host_ + 
                " - CLIENT_READ_ERROR IS COMING SOON..."
                ,Logger::LOG_LEVEL::FATAL
            );
            client_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
            client_socket_.close();
        }
    }
}

void Bridge::print_error_source(SOCKET_ERROR_SOURCE error_source)
{
    switch(error_source)
    {
        case Bridge::SOCKET_ERROR_SOURCE::CLIENT_READ_ERROR:
        {
            Logger::log("CLIENT_READ_ERROR", Logger::LOG_LEVEL::WARNING);
            break; 
        }
        case Bridge::SOCKET_ERROR_SOURCE::CLIENT_WRITE_ERROR:
        {
            Logger::log("CLIENT_WRITE_ERROR", Logger::LOG_LEVEL::WARNING);
            break; 
        }
        case Bridge::SOCKET_ERROR_SOURCE::SERVER_CONNECT_ERROR:
        {
            Logger::log("SERVER_CONNECT_ERROR", Logger::LOG_LEVEL::WARNING);
            break; 
        }
        case Bridge::SOCKET_ERROR_SOURCE::SERVER_READ_ERROR:
        {
            Logger::log("SERVER_READ_ERROR", Logger::LOG_LEVEL::WARNING);
            break; 
        }
        case Bridge::SOCKET_ERROR_SOURCE::SERVER_WRITE_ERROR:
        {
            Logger::log("SERVER_WRITE_ERROR", Logger::LOG_LEVEL::WARNING);
            break; 
        }   
    }
}

Bridge::~Bridge(){
      Logger::log("d'tor of Bridge ", Logger::LOG_LEVEL::FATAL);
  }