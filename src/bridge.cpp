#include "bridge.hpp"

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/write.hpp>

template <typename SocketType>
Bridge<SocketType>::Bridge(std::shared_ptr<boost::asio::io_context> io_context)
  : io_context_(io_context),
    strand_(*io_context){}

template <typename SocketType>
void Bridge<SocketType>::start_by_connect(char client_buffer [max_data_length],
                                          const boost::system::error_code& error,
                                          std::size_t bytes_transferred,
                                          endpoint_type endpoint,
                                          const std::string& domain)
{
    if(error) return;

    client_host_ = boost::lexical_cast<std::string>(client_socket_->lowest_layer().remote_endpoint());

    // Choose to save the message for future forwarding, or not
    save_first_message(client_buffer);

    Logger::log(
        "First domain for this Bridge\nAttempting to connect to " + domain + 
        " [C] " + boost::lexical_cast<std::string>(client_socket_->lowest_layer().remote_endpoint()),
        Logger::LOG_LEVEL::INFO
    );

    do_handshake(client_socket_, boost::asio::ssl::stream_base::server);

    // Create a new server socket and insert to the map for future reuse
    std::shared_ptr<SocketType> new_server_socket = create_new_server_socket();
    server_socket_map_[boost::lexical_cast<std::string>(endpoint)] = new_server_socket;

    new_server_socket->lowest_layer().async_connect(
        endpoint,
        boost::bind(
            &Bridge::handle_server_connect,
            this->shared_from_this(),
            new_server_socket,
            boost::asio::placeholders::error,
            boost::lexical_cast<std::string>(endpoint)
        )
    ); 
}
  
template <typename SocketType>
void Bridge<SocketType>::handle_server_connect(std::shared_ptr<SocketType> server_socket,
                                               const boost::system::error_code& error,
                                               const std::string& server_host)
{
    if(error)
    {
        strand_.post(
            boost::bind(
                &Bridge::close,
                this->shared_from_this(),
                server_socket, 
                Bridge::SOCKET_ERROR_SOURCE::SERVER_CONNECT_ERROR, 
                server_host, 
                error
            )
        );
        return;
    }

    Logger::log(
        "Connection established with the requested server. [C] " + 
        client_host_ + "  [S] " + server_host,
        Logger::LOG_LEVEL::INFO
    );    

    do_handshake(server_socket, boost::asio::ssl::stream_base::client);
    // Setup async read from remote server (server). Preperation for the following write
    server_socket->async_read_some(
        boost::asio::buffer(server_buffer_,max_data_length),
        boost::bind(
            &Bridge::handle_server_read,
            this->shared_from_this(),
            server_socket,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred,
            server_host
        )
    );

    // Send the first messaage to the server
    // Only if got a message to forward - e.g Https first message that we get is CONNECT 
    // We dont want to forward it so we are not saving it in client_buffer_
    if(strlen(client_buffer_) > 0)
    {
        async_write(
            *server_socket,
            boost::asio::buffer(client_buffer_, strlen(client_buffer_)),
            boost::bind(
                &Bridge::handle_server_write,
                this->shared_from_this(),
                server_socket,
                boost::asio::placeholders::error,
                server_host
            )
        );
    }
    // In case that derived bridge hasnt saved the first message, don't forward it
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
                server_host
            )
        );
    }
}

// Reading from client is completed, now send the data to the remote server
template <typename SocketType>
void Bridge<SocketType>::handle_client_read(std::shared_ptr<SocketType> server_socket, 
                                            const boost::system::error_code& error,
                                            std::size_t bytes_transferred,
                                            const std::string& server_host)
{
    if(error)
    {
        strand_.post(
            boost::bind(
                &Bridge::close,
                this->shared_from_this(),
                server_socket, 
                Bridge::SOCKET_ERROR_SOURCE::CLIENT_READ_ERROR, 
                server_host, 
                error
            )
        );
        return;
    }

    Logger::log(
        "Client --> Proxy     Server.   [C] " + 
        client_host_ + "  [S] " + server_host,
        Logger::LOG_LEVEL::INFO
    );
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);
    
    // Do tests on the request to check if Valid or Forbidden
    std::regex re("((\\d+[.]){3}\\d+)[:](\\d+)");
    std::smatch cl_info,server_info; 
    std::regex_search(client_host_, cl_info, re);
    std::regex_search(server_host, server_info, re);

	if (isForbidden(cl_info[1], server_info[1], cl_info[3], server_info[3], client_buffer_))
	{
		strand_.post(boost::bind(&Bridge::close, this->shared_from_this(), server_socket, Bridge::SOCKET_ERROR_SOURCE::FORBIDDEN_REQUEST, server_host, error.message()));
		return;
	}


    // Resolve the remote host (If it appeared in the message)
    std::string domain;
    int parsing_error = Utils::parse_domain(
        boost::lexical_cast<std::string>(client_buffer_), domain
    );
    
    // No new Host was provided in the message - use the current one (In case of a regex error)
    if(parsing_error)
    {
        async_write(
            *server_socket,
            boost::asio::buffer(client_buffer_,bytes_transferred),
            boost::bind(
                &Bridge::handle_server_write,
                this->shared_from_this(),
                server_socket,
                boost::asio::placeholders::error,
                server_host
            )
        );
        return;
    } 

    // Get the server socket associated with the remote host
    endpoint_type requested_endpoint  = Utils::resolve_endpoint(
        domain, *io_context_
    );

    // No new Host was provided in the message - use the current one ( the domain could not be translated to an endpoint )
    if(boost::lexical_cast<std::string>(requested_endpoint) == ENDPOINT_ADDRESS_ERROR)
    {
        async_write(
            *server_socket,
            boost::asio::buffer(client_buffer_,bytes_transferred),
            boost::bind(
                &Bridge::handle_server_write,
                this->shared_from_this(),
                server_socket,
                boost::asio::placeholders::error,
                server_host
            )
        );
    } 

    // New Host was written in the message - The endpoint is valid
    else
    {
        auto cached_server_socket = server_socket_map_.find(boost::lexical_cast<std::string>(requested_endpoint));

        // Check if there is already a socket for this endpoint
        if(cached_server_socket == server_socket_map_.end()) // doesnt exist yet
        {
            Logger::log(
                "New domain for this Bridge, wasn't saved before\nAttempting to connect to " + domain + 
                " [C] " + client_host_,
                Logger::LOG_LEVEL::INFO
            );

            // Create a new socket and start using it
            std::shared_ptr<SocketType> new_server_socket = create_new_server_socket();
            server_socket_map_[boost::lexical_cast<std::string>(requested_endpoint)] = new_server_socket;

            new_server_socket->lowest_layer().async_connect(
                requested_endpoint,
                boost::bind(
                    &Bridge::handle_server_connect,
                    this->shared_from_this(),
                    new_server_socket,
                    error,
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
                    server_host
                )
            );
        }
    }
}

// Write to remote server complete, Async read from client
template <typename SocketType>
void Bridge<SocketType>::handle_server_write(std::shared_ptr<SocketType> server_socket,
                                             const boost::system::error_code& error,
                                             const std::string& server_host)
{
    if(error) 
    {
        strand_.post(
            boost::bind(
                &Bridge::close,
                this->shared_from_this(), 
                server_socket, 
                Bridge::SOCKET_ERROR_SOURCE::SERVER_WRITE_ERROR, 
                server_host, 
                error
            )
        );
        return;
    }

    Logger::log(
        "Client     Proxy --> Server.   [C] " + 
        client_host_ + "  [S] " + server_host,
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
            server_host
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
                server_host
            )
        );
    }
    // This is an https bridge so no need to send to the server the first message
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
                server_host
            )
        );
    }
}

// Reading from client is completed, now send the data to the remote server
template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::handle_client_read(std::shared_ptr<SocketType> server_socket, 
                                const boost::system::error_code& error,
                                std::size_t bytes_transferred,
                                const std::string& server_host)
{
    if(error)
    {
        strand_.post(boost::bind (&Bridge::close,this->shared_from_this(),server_socket, Bridge::SOCKET_ERROR_SOURCE::CLIENT_READ_ERROR, server_host, error.message()));
        return;
    }

    Logger::log(
        "Client --> Proxy     Server.   [C] " + 
        client_host_ + "  [S] " + server_host,
        Logger::LOG_LEVEL::INFO
    );
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);
    
    // Resolve the remote host (If appeared in the message)
    std::string domain;
    int parsing_error = Utils::parse_domain(
        boost::lexical_cast<std::string>(client_buffer_), domain
    );

    // Do tests on the request to check if Valid or Forbidden
    std::regex re("((\\d+[.]){3}\\d+)[:](\\d+)");
    std::smatch cl_info,server_info; 
    std::regex_search(client_host_, cl_info, re);
    std::regex_search(server_host, server_info, re);

	if (isForbidden(cl_info[1], server_info[1], cl_info[3], server_info[3], client_buffer_))
	{
		strand_.post(boost::bind(&Bridge::close, this->shared_from_this(), server_socket, Bridge::SOCKET_ERROR_SOURCE::FORBIDDEN_REQUEST, server_host, error.message()));
		return;
	}

    // No new Host was provided in the message - use the current one (In case of a regex error)
    if(parsing_error)
    {
        async_write(
            *server_socket,
            boost::asio::buffer(client_buffer_,bytes_transferred),
            boost::bind(
                &Bridge::handle_server_write,
                this->shared_from_this(),
                server_socket,
                boost::asio::placeholders::error,
                server_host
            )
        );
        return;
    } 

    // Get the server socket associated with the remote host
    endpoint_type requested_endpoint  = Utils::resolve_endpoint(
        domain, *io_context_
    );

    // No new Host was provided in the message - use the current one ( the domain could not be translated to an endpoint )
    if(boost::lexical_cast<std::string>(requested_endpoint) == ENDPOINT_ADDRESS_ERROR)
    {
        async_write(
            *server_socket,
            boost::asio::buffer(client_buffer_,bytes_transferred),
            boost::bind(
                &Bridge::handle_server_write,
                this->shared_from_this(),
                server_socket,
                boost::asio::placeholders::error,
                server_host
            )
        );
    } 

    // New Host was written in the message - The endpoint is valid
    else
    {
        auto cached_server_socket = server_socket_map_.find(boost::lexical_cast<std::string>(requested_endpoint));

        // Check if there is already a socket for this endpoint
        if(cached_server_socket == server_socket_map_.end()) // doesnt exist yet
        {
            Logger::log(
                "New domain for this Bridge, wasn't saved before\nAttempting to connect to " + domain + 
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
                    server_host
                )
            );
        }
    }
}

// Write to remote server complete, Async read from client
template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::handle_server_write(std::shared_ptr<SocketType> server_socket,
                                 const boost::system::error_code& error,
                                 const std::string& server_host)
{
    if(error) 
    {
        strand_.post(boost::bind (&Bridge::close,this->shared_from_this(), server_socket, Bridge::SOCKET_ERROR_SOURCE::SERVER_WRITE_ERROR, server_host, error.message()));
        return;
    }

    Logger::log(
        "Client     Proxy --> Server.   [C] " + 
        client_host_ + "  [S] " + server_host,
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
            server_host
        )
    );
}

// Read from client complete, now send data to remote server
template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::handle_server_read(std::shared_ptr<SocketType> server_socket,
                                const boost::system::error_code& error,
                                const size_t& bytes_transferred,
                                const std::string& server_host)
{
    if(error) {
        strand_.post(boost::bind (&Bridge::close,this->shared_from_this(), server_socket, Bridge::SOCKET_ERROR_SOURCE::SERVER_READ_ERROR, server_host, error.message()));
        return;
    } 
    
    Logger::log(
        "Client     Proxy <-- Server.   [C] " + 
        client_host_ + "  [S] " + server_host,
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
            server_host
        )
    );
}

template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::handle_client_write(std::shared_ptr<SocketType> server_socket,
                                 const boost::system::error_code& error,
                                 const std::string& server_host)
{
    if(error) 
    {
        strand_.post(boost::bind (&Bridge::close,this->shared_from_this(), server_socket, Bridge::SOCKET_ERROR_SOURCE::CLIENT_WRITE_ERROR, server_host, error.message())); 
        return;
    }

    Logger::log(
        "Client <-- Proxy     Server.   [C] " + 
        client_host_ + "  [S] " + server_host,
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
            server_host
        )
    );
}

template <class BridgeType, typename SocketType>
void Bridge<BridgeType, SocketType>::close(std::shared_ptr<SocketType> server_socket,  
                   SOCKET_ERROR_SOURCE error_source,
                   const std::string& server_host,
                   const std::string& error_message)
{
    //print the error origin
    Bridge::print_error_source(error_source, server_host, error_message);

    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;

    // If the error is in the client socket, close it as well as all server sockets
    if( error_source == Bridge::SOCKET_ERROR_SOURCE::CLIENT_WRITE_ERROR || 
        error_source == Bridge::SOCKET_ERROR_SOURCE::CLIENT_READ_ERROR ||
		error_source == Bridge::SOCKET_ERROR_SOURCE::FORBIDDEN_REQUEST )
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
            "Deleting [S] " + server_host + " for [C]",
            Logger::LOG_LEVEL::DEBUG
        );
        server_socket_map_.erase(server_host);
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
void Bridge<BridgeType, SocketType>::print_error_source(SOCKET_ERROR_SOURCE error_source, 
                                                        const std::string& server_host,
                                                        const std::string& error_message)
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
            Logger::log("SERVER_CONNECT_ERROOR, Please make sure the host is reachable", Logger::LOG_LEVEL::WARNING);
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
        case Bridge::SOCKET_ERROR_SOURCE::FORBIDDEN_REQUEST:
        {
            Logger::log("FORBIDDEN_REQUEST", Logger::LOG_LEVEL::FATAL);
            break;
        }
    }
    Logger::log("ERROR: " + error_message + " [C] " + client_host_ + " [S] " + server_host, Logger::LOG_LEVEL::WARNING);
}

template class Bridge<HttpBridge, HttpSocketType>;
template class Bridge<HttpsBridge, SslStreamType>;
template class Bridge<FtpBridge, HttpSocketType>;
// Read from client complete, now send data to remote server
template <typename SocketType>
void Bridge<SocketType>::handle_server_read(std::shared_ptr<SocketType> server_socket,
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

template <typename SocketType>
void Bridge<SocketType>::handle_client_write(std::shared_ptr<SocketType> server_socket,
                                             const boost::system::error_code& error,
                                             const std::string& server_host)
{
    if(error) 
    {
        strand_.post(
            boost::bind(
                &Bridge::close,
                this->shared_from_this(), 
                server_socket, 
                Bridge::SOCKET_ERROR_SOURCE::CLIENT_WRITE_ERROR, 
                server_host, 
                error
            )
        ); 
        return;
    }

    Logger::log(
        "Client <-- Proxy     Server.   [C] " + 
        client_host_ + "  [S] " + server_host,
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
            server_host
        )
    );
}

template <typename SocketType>
void Bridge<SocketType>::close(std::shared_ptr<SocketType> server_socket,  
                               SOCKET_ERROR_SOURCE error_source,
                               const std::string& server_host,
                               const boost::system::error_code& error)
{

    if(error.value() == boost::system::errc::operation_canceled) return;

    //print the error origin in it is not boost::system::errc::operation_canceled
    Bridge::print_error_source(error_source, server_host, error);

    // If the error is in the client socket, close it as well as all server sockets
    if( (error_source == Bridge::SOCKET_ERROR_SOURCE::CLIENT_WRITE_ERROR || 
         error_source == Bridge::SOCKET_ERROR_SOURCE::CLIENT_READ_ERROR||
         error_source == Bridge::SOCKET_ERROR_SOURCE::FORBIDDEN_REQUEST)
    )
    {
        if(client_socket_->lowest_layer().is_open())
        {
            close_socket(client_socket_);
        }
        for(auto& server_socket_iter: server_socket_map_)
        {
            close_socket(server_socket_iter.second);
        }
        server_socket_map_.clear();
    }
    
    //  Whe upgrading a beidge to be secure, no need to close the client socket on which we accepted the connection
    else if(error_source == Bridge::SOCKET_ERROR_SOURCE::BRIDGE_UPGRADED)
    {
        for(auto& server_socket_iter: server_socket_map_)
        {
            close_socket(server_socket_iter.second);
        }
        server_socket_map_.clear();
    }
    
    // Error is with one of the server sockets in the map
    else if(server_socket->lowest_layer().is_open())
    {
        Logger::log(
            "Deleting [S] " + server_host + " for [C]",
            Logger::LOG_LEVEL::DEBUG
        );

        server_socket_map_.erase(server_host);
        close_socket(server_socket);

        // if it is the last server socket for this bridge, clear the client socket and close the bridge
        if(server_socket_map_.size() == 0)
        {
            Logger::log(
                "All servers are gone, clearing up this bridge [C] " +
                client_host_,
                Logger::LOG_LEVEL::INFO
            );
            close_socket(client_socket_);
        }
    }

}

template <typename SocketType>
void Bridge<SocketType>::print_error_source(SOCKET_ERROR_SOURCE error_source, 
                                            const std::string& server_host,
                                            const boost::system::error_code& error)
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
            Logger::log("SERVER_CONNECT_ERROOR, Please make sure the host is reachable", Logger::LOG_LEVEL::WARNING);
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
        case Bridge::SOCKET_ERROR_SOURCE::BRIDGE_UPGRADED:
        {
            Logger::log("BRIDGE_UPGRADED_TO_SECURE_BRIDGE", Logger::LOG_LEVEL::WARNING);
            return; 
        }
         case Bridge::SOCKET_ERROR_SOURCE::FORBIDDEN_REQUEST:
        {
            Logger::log("FORBIDDEN_REQUEST", Logger::LOG_LEVEL::FATAL);
            break;
        }
    }
    Logger::log("ERROR: " + error.message() + " [C] " + client_host_ + " [S] " + server_host, Logger::LOG_LEVEL::WARNING);
}

template <typename SocketType>
void Bridge<SocketType>::do_handshake(std::shared_ptr<SocketType> socket,
                                      boost::asio::ssl::stream_base::handshake_type handshake_type)
{
    return;
}

template <typename SocketType>
void Bridge<SocketType>::save_first_message(char client_buffer [max_data_length])
{
    return;
}

// Default implementation for non-secure Bridges
template <typename SocketType>
void Bridge<SocketType>::close_socket(std::shared_ptr<SocketType> socket)
{
    boost::system::error_code ignored_ec;
    socket->lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
    socket.reset();
}

template class Bridge<HttpSocketType>;
template class Bridge<SslStreamType>;