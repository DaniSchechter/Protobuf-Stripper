#include "bridge.hpp"

#include "httpBridge.hpp"
#include "httpsBridge.hpp"

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/write.hpp>

template <class BridgeType, typename SocketType>
Bridge<BridgeType, SocketType>::Bridge(std::shared_ptr<boost::asio::io_context> io_context)
  : io_context_(io_context),
    strand_(*io_context){}

template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::start_by_connect(char client_buffer [max_data_length],
                                                    const boost::system::error_code& error,
                                                    std::size_t bytes_transferred,
                                                    endpoint_type endpoint,
                                                    const std::string& domain)
{
    derrived_bridge_type()->start_by_connect(client_buffer,
                                            error,
                                            bytes_transferred,
                                            endpoint,
                                            domain);
}

template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::handle_server_connect( 
    std::shared_ptr<SocketType> server_socket,
    const boost::system::error_code& error,
    std::size_t bytes_transferred,
    const std::string& endpoint)
{
    if(error)
    {
        Logger::log("Error in SERVER_CONNECT: " + error.message(), Logger::LOG_LEVEL::WARNING);
        // TODO think of edge cases
        close(server_socket, Bridge::SOCKET_ERROR_SOURCE::SERVER_CONNECT_ERROR, endpoint);
        return;
    }

    Logger::log(
        "Connection established with the requested server. [C] " + 
        client_host_ + "  [S] " + endpoint,
        Logger::LOG_LEVEL::INFO
    );    

    derrived_bridge_type()->do_handshake(server_socket, boost::asio::ssl::stream_base::client);
    // Setup async read from remote server (server). Preperation for the following write
    server_socket->async_read_some(
        boost::asio::buffer(server_buffer_,max_data_length),
        boost::bind(
            &Bridge::handle_server_read,
            this->shared_from_this(),
            server_socket,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred,
            endpoint
        )
    );

    // Send the first messaage to the server
    // Only if got a message to forward - e.g Https first message that we get is CONNECT 
    // We dont want to forward it so we are not saving it in client_buffer_

    if(strlen(client_buffer_) > 0)
    {
        async_write(
            *server_socket,
            boost::asio::buffer(client_buffer_,bytes_transferred),
            boost::bind(
                &Bridge::handle_server_write,
                this->shared_from_this(),
                server_socket,
                boost::asio::placeholders::error,
                endpoint
            )
        );
    }
    else
    {
        client_socket_->async_read_some(
            boost::asio::buffer(client_buffer_,max_data_length),
            boost::bind(
                &Bridge::handle_client_read,
                this->shared_from_this(),
                server_socket,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred,
                endpoint
            )
        );
    }
}

// Reading from client is completed, now send the data to the remote server
template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::handle_client_read(std::shared_ptr<SocketType> server_socket, 
                                const boost::system::error_code& error,
                                std::size_t bytes_transferred,
                                const std::string& endpoint)
{
    if(error)
    {
        Logger::log("Error in CLIENT_READ: " + error.message(), Logger::LOG_LEVEL::WARNING);
        Logger::log("From [C] " + client_host_ + " To " + endpoint, Logger::LOG_LEVEL::WARNING);
            close(server_socket, Bridge::SOCKET_ERROR_SOURCE::CLIENT_READ_ERROR, endpoint);
            return;
    }

    Logger::log(
        "Client --> Proxy     Server.   [C] " + 
        client_host_ + "  [S] " + endpoint,
        Logger::LOG_LEVEL::INFO
    );
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);
    
    // Resolve the remote host (If appeared in the message)
    std::string domain;
    int parsing_error = Utils::parse_domain(
        boost::lexical_cast<std::string>(client_buffer_), domain
    );
    
    // No new Host was provided in the message - use the current one
    if (parsing_error)
    {
        // Check if the request's domain is empty
        if (parsing_error == Utils::EMPTY_ABSOLUTE_URI)
        {
            const char* s = Utils::generate_absolute_uri_request(boost::lexical_cast<std::string>(client_buffer_));
            strncpy(client_buffer_, s, strlen(s)); 
        }
        async_write(
            *server_socket,
            boost::asio::buffer(client_buffer_,bytes_transferred),
            boost::bind(
                &Bridge::handle_server_write,
                this->shared_from_this(),
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
        endpoint_type requested_endpoint  = Utils::resolve_endpoint(
            domain, *io_context_
        );

        if(requested_endpoint.address().to_string() == ENDPOINT_ADDRESS_ERROR) 
        { 
            Logger::log(
                "Got a new domain: " + domain + ", and could not resolve it to an endpoint",
                Logger::LOG_LEVEL::INFO
            );
            return;
            // Read again, since we was provided with a new domain so we can't sent the message to the prev server
            //TODO call the read again
        }

        auto cached_server_socket = server_socket_map_.find(boost::lexical_cast<std::string>(requested_endpoint));

        // Check if there is already a socket for this endpoint
        if(cached_server_socket == server_socket_map_.end()) // doesnt exist yet
        {
            Logger::log(
                "2) Attempting to connect to " + domain + 
                " [C] " + client_host_,
                Logger::LOG_LEVEL::INFO
            );

            // Create a new socket and start using it
            std::shared_ptr<SocketType> new_server_socket = derrived_bridge_type()->create_new_server_socket();
            server_socket_map_[boost::lexical_cast<std::string>(requested_endpoint)] = new_server_socket;

            new_server_socket->lowest_layer().async_connect(
                requested_endpoint,
                boost::bind(
                    &Bridge::handle_server_connect,
                    this->shared_from_this(),
                    new_server_socket,
                    error,
                    bytes_transferred,
                    boost::lexical_cast<std::string>(requested_endpoint)
                )
            );
        }

        // If there is a socket, continue in the cycle
        else
        {
            async_write(
                *(cached_server_socket->second),
                boost::asio::buffer(client_buffer_,bytes_transferred),
                boost::bind(
                    &Bridge::handle_server_write,
                    this->shared_from_this(),
                    cached_server_socket->second,
                    boost::asio::placeholders::error,
                    endpoint
                )
            );
        }
    }
}

// Write to remote server complete, Async read from client
template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::handle_server_write(std::shared_ptr<SocketType> server_socket,
                                 const boost::system::error_code& error,
                                 const std::string& endpoint)
{
    if(error) 
    {
        Logger::log("Error in SERVER_WRITE: " + error.message(), Logger::LOG_LEVEL::WARNING);
        close(server_socket, Bridge::SOCKET_ERROR_SOURCE::SERVER_WRITE_ERROR, endpoint);
        return;
    }

    Logger::log(
        "Client     Proxy --> Server.   [C] " + 
        client_host_ + "  [S] " + endpoint,
        Logger::LOG_LEVEL::INFO
    );
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);

    client_socket_->async_read_some(
        boost::asio::buffer(client_buffer_,max_data_length),
        boost::bind(
            &Bridge::handle_client_read,
            this->shared_from_this(),
            server_socket,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred,
            endpoint
        )
    );
}

// Read from client complete, now send data to remote server
template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::handle_server_read(std::shared_ptr<SocketType> server_socket,
                                const boost::system::error_code& error,
                                const size_t& bytes_transferred,
                                const std::string& endpoint)
{
    if(error) {
        Logger::log("Error in SERVER_READ: " + error.message(), Logger::LOG_LEVEL::WARNING);
        close(server_socket, Bridge::SOCKET_ERROR_SOURCE::SERVER_READ_ERROR, endpoint);
        return;
    } 
    
    Logger::log(
        "Client     Proxy <-- Server.   [C] " + 
        client_host_ + "  [S] " + endpoint,
        Logger::LOG_LEVEL::INFO
    );
    Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::DEBUG);

    async_write(
        *client_socket_,
        boost::asio::buffer(server_buffer_,bytes_transferred),
        boost::bind(
            &Bridge::handle_client_write,
            this->shared_from_this(),
            server_socket,
            boost::asio::placeholders::error,
            endpoint
        )
    );
}

template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::handle_client_write(std::shared_ptr<SocketType> server_socket,
                                 const boost::system::error_code& error,
                                 const std::string& endpoint)
{
    if(error) 
    {
        Logger::log("Error in CLIENT_WRITE: " + error.message(), Logger::LOG_LEVEL::WARNING);
        close(server_socket, Bridge::SOCKET_ERROR_SOURCE::CLIENT_WRITE_ERROR, endpoint); 
        return;
    }

    Logger::log(
        "Client <-- Proxy     Server.   [C] " + 
        client_host_ + "  [S] " + endpoint,
        Logger::LOG_LEVEL::INFO
    );

    Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::DEBUG);
           
    server_socket->async_read_some(
        boost::asio::buffer(server_buffer_,max_data_length),
        boost::bind(
            &Bridge::handle_server_read,
            this->shared_from_this(),
            server_socket,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred,
            endpoint
        )
    );
}

template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::close(std::shared_ptr<SocketType> server_socket,  
                   SOCKET_ERROR_SOURCE error_source,
                   const std::string& endpoint)
{
    //print the error origin
    Bridge::print_error_source(error_source);

    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;

    // If the error is in the client socket, close it as well as all server sockets
    if( error_source == Bridge::SOCKET_ERROR_SOURCE::CLIENT_WRITE_ERROR || 
        error_source == Bridge::SOCKET_ERROR_SOURCE::CLIENT_READ_ERROR )
    {
        if (client_socket_->lowest_layer().is_open())
        {
            client_socket_->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
            // client_socket_->close();
        }
        for(auto& server_socket_iter: server_socket_map_)
        {
            server_socket_iter.second->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
            server_socket_iter.second.reset();
        }
        server_socket_map_.clear();
    }

    
    // Error is with one of the server sockets in the map
    else if(server_socket->lowest_layer().is_open())
    {
        Logger::log(
            "Deleting [S] " + endpoint + " for [C]",
            Logger::LOG_LEVEL::DEBUG
        );

        server_socket_map_.erase(endpoint);

        // TODO if the erase raises exception we need to find the server socket and erase using the iterator
        server_socket->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        server_socket.reset();

        // if it is the last server socket for this bridge, clear the client socket and close the bridge
        if(server_socket_map_.size() == 0)
        {
            Logger::log(
                "All servers are gone, clearing up this bridge [C] " +
                client_host_,
                Logger::LOG_LEVEL::INFO
            );
            client_socket_->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
            // client_socket_->lower.close();
        }
    }
}

template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::print_error_source(SOCKET_ERROR_SOURCE error_source)
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

template <class BridgeType, typename SocketType>
Bridge<BridgeType, SocketType>::~Bridge<BridgeType, SocketType>()
{
      Logger::log("D'tor of Bridge ", Logger::LOG_LEVEL::INFO);
}

template class Bridge<HttpBridge, HttpSocketType>;
template class Bridge<HttpsBridge, SslStreamType>;

// boost::asio::ssl::stream<boost::asio::basic_stream_socket<boost::asio::ip::tcp> >::stream(boost::asio::io_context&)