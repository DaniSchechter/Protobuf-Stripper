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
    std::cout <<"@@@@@@@@@@@" <<std::endl;
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
    std::cout <<"$$$$$$$$$$$$$$$$" <<std::endl;
}

// // Hanadle first packet read from the client, resolve it's destination and continue in communication cycle
// void Bridge::connect_after_read(socket_type& server_socket,
//                                 const boost::system::error_code& error,
//                                 std::size_t bytes_transferred)
// {   

//     // Resolve the absolute-uri from the first buffer got from client
//     boost::asio::ip::tcp::endpoint endpoint = Request_parser::resolve_host(
//         boost::lexical_cast<std::string>(client_buffer_),
//         io_context_
//     );
    
//     Logger::log("Attempting to connect to "+ boost::lexical_cast<std::string>(endpoint), Logger::LOG_LEVEL::INFO);

    
//     // Attempt connection to remote server (server side)
//     server_socket.async_connect(
//         endpoint,
//         boost::bind(
//             handle_server_connect,
//             shared_from_this(),
//             boost::asio::placeholders::error
//         )
//     );

// }
void Bridge::handle_server_connect(std::shared_ptr<socket_type> server_socket,
                                   const boost::system::error_code& error,
                                   std::size_t bytes_transferred)
{

    std::cout << "handle_server_connect" <<std::endl;
    if(error)
    {
        std::cout << "111111" <<std::endl;
        // TODO think of edge cases
        close(server_socket, Bridge::SOCKET_ERROR_SOURCE::SERVER_ERROR);
        return;
    }

    Logger::log(
        "Connection established.   [C] " + 
        boost::lexical_cast<std::string>(client_socket_.remote_endpoint()) + "  [S] " +
        boost::lexical_cast<std::string>(server_socket->remote_endpoint()),
        Logger::LOG_LEVEL::WARNING
    );

    // If no
    if (!error)
    {
        // Setup async read from remote server (server)
        server_socket->async_read_some(
            boost::asio::buffer(server_buffer_,max_data_length),
            boost::bind(
                &Bridge::handle_server_read,
                shared_from_this(),
                server_socket,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
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
                boost::asio::placeholders::error
            )
        );
    }
}
void Bridge::handle_client_read(const boost::system::error_code& error,
                                std::size_t bytes_transferred)
{
    std::cout << "handle_client_read" <<std::endl;
     Logger::log(
         "Client --> Proxy     Server.   [C] " + 
        boost::lexical_cast<std::string>(client_socket_.remote_endpoint()) + "  [Prev S] First read from this client" ,
        Logger::LOG_LEVEL::WARNING
    );
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);
    
    // Resolve the endpoint
    boost::asio::ip::tcp::endpoint endpoint = Request_parser::resolve_host(
        boost::lexical_cast<std::string>(client_buffer_),
        io_context_
    );

    // If the endpoint is not valid
    if (boost::lexical_cast<std::string>(endpoint) == "0.0.0.0:0")
    {
        return;
    }

    // Get the server socket associated with the remote host
    auto server_socket = server_socket_map_.find(boost::lexical_cast<std::string>(endpoint.address()));
    // Check if there is already a socket for this endpoint
    if(server_socket == server_socket_map_.end()) // doesnt exist yet
    {
        Logger::log(
            "1) Attempting to connect to "+ boost::lexical_cast<std::string>(endpoint) + 
            " [C] " + boost::lexical_cast<std::string>(client_socket_.remote_endpoint()),
            Logger::LOG_LEVEL::INFO
        );
        server_socket_map_[boost::lexical_cast<std::string>(endpoint.address())] = std::make_shared<socket_type>((*io_context_));
        std::shared_ptr<socket_type> new_server_socket = server_socket_map_[boost::lexical_cast<std::string>(endpoint.address())];
        new_server_socket->async_connect(
            endpoint,
            boost::bind(
                &Bridge::handle_server_connect,
                shared_from_this(),
                new_server_socket,
                error,
                bytes_transferred
            )
        );
    }

    // If there is a socket continue in the cycle
    else if (!error)
    {
        async_write(
            *(server_socket->second),
            boost::asio::buffer(client_buffer_,bytes_transferred),
            boost::bind(
                &Bridge::handle_server_write,
                shared_from_this(),
                server_socket->second,
                boost::asio::placeholders::error
            )
        );
    }

    else
    {
        std::cout << "2222222" <<std::endl;
        close(server_socket->second, Bridge::SOCKET_ERROR_SOURCE::CLIENT_ERROR);
        return;
    }
}


// Read from client complete, now send data to remote server
void Bridge::handle_client_read(std::shared_ptr<socket_type> server_socket, 
                                const boost::system::error_code& error,
                                std::size_t bytes_transferred)
{
    std::cout << "handle_client_read" <<std::endl;
    if(error)
    {
            std::cout << "333333" <<std::endl;
            close(server_socket, Bridge::SOCKET_ERROR_SOURCE::CLIENT_ERROR);
            std::cout << "!!!!!!!!!!!!!!!!!!!" <<std::endl;
    }

    else
    {
        Logger::log(
        "Client --> Proxy     Server.   [C] " + 
        boost::lexical_cast<std::string>(client_socket_.remote_endpoint()) + "  [S] " +
        boost::lexical_cast<std::string>(server_socket->remote_endpoint()),
        Logger::LOG_LEVEL::WARNING
    );
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);
    
    // Resolve the endpoint
    boost::asio::ip::tcp::endpoint endpoint = Request_parser::resolve_host(
        boost::lexical_cast<std::string>(client_buffer_),
        io_context_
    );

    // If the endpoint is not valid
    if (boost::lexical_cast<std::string>(endpoint) == "0.0.0.0:0")
    {
        async_write(
            *server_socket,
            boost::asio::buffer(client_buffer_,bytes_transferred),
            boost::bind(
                &Bridge::handle_server_write,
                shared_from_this(),
                server_socket,
                boost::asio::placeholders::error
            )
        );
    }
    // The endpoint is valid
    else
    {
        // Get the server socket associated with the remote host
        auto server_socket = server_socket_map_.find(boost::lexical_cast<std::string>(endpoint.address()));
        // Check if there is already a socket for this endpoint
        if(server_socket == server_socket_map_.end()) // doesnt exist yet
        {
            Logger::log(
                "2) Attempting to connect to "+ boost::lexical_cast<std::string>(endpoint) + 
                " [C] " + boost::lexical_cast<std::string>(client_socket_.remote_endpoint()),
                Logger::LOG_LEVEL::INFO
            );
            server_socket_map_[boost::lexical_cast<std::string>(endpoint.address())] = std::make_shared<socket_type>((*io_context_));
            std::shared_ptr<socket_type> new_server_socket = server_socket_map_[boost::lexical_cast<std::string>(endpoint.address())];
            new_server_socket->async_connect(
                endpoint,
                boost::bind(
                    &Bridge::handle_server_connect,
                    shared_from_this(),
                    new_server_socket,
                    error,
                    bytes_transferred
                )
            );
        }

        // If there is a socket continue in the cycle
        else if (!error)
        {
            async_write(
                *(server_socket->second),
                boost::asio::buffer(client_buffer_,bytes_transferred),
                boost::bind(
                    &Bridge::handle_server_write,
                    shared_from_this(),
                    server_socket->second,
                    boost::asio::placeholders::error
                )
            );
        }
    }
    }
    
    
}




// Write to remote server complete, Async read from client
void Bridge::handle_server_write(std::shared_ptr<socket_type> server_socket,
                                 const boost::system::error_code& error)
{
    std::cout << "handle_server_write" <<std::endl;
    if(error) 
    {
        std::cout << "44444" <<std::endl;
        close(server_socket, Bridge::SOCKET_ERROR_SOURCE::SERVER_ERROR);
        return;
    }

    Logger::log(
        "Client     Proxy --> Server.   [C] " + 
        boost::lexical_cast<std::string>(client_socket_.remote_endpoint()) + "  [S] " +
        boost::lexical_cast<std::string>(server_socket->remote_endpoint()),
        Logger::LOG_LEVEL::WARNING
    );
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);

    if (!error)
    {
        client_socket_.async_read_some(
            boost::asio::buffer(client_buffer_,max_data_length),
            boost::bind(
                &Bridge::handle_client_read,
                shared_from_this(),
                server_socket,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );
        std::cout << "%%%%%%%%%%%%%%" << std::endl;
    }
}

// Read from client complete, now send data to remote server
void Bridge::handle_server_read(std::shared_ptr<socket_type> server_socket,
                                const boost::system::error_code& error,
                                const size_t& bytes_transferred)
{
    std::cout << "handle_server_read" <<std::endl;

    if(error) {
        Logger::log(
        "Client     Proxy <-- Server.   [C] " + 
        boost::lexical_cast<std::string>(client_socket_.remote_endpoint()) + "  [S] " +
        boost::lexical_cast<std::string>(server_socket->remote_endpoint()),
        Logger::LOG_LEVEL::WARNING
    );
        std::cout << "555555" << server_socket->is_open() << std::endl;
        close(server_socket, Bridge::SOCKET_ERROR_SOURCE::SERVER_ERROR);
        return;
    } 
    Logger::log(
        "Client     Proxy <-- Server.   [C] " + 
        boost::lexical_cast<std::string>(client_socket_.remote_endpoint()) + "  [S] " +
        boost::lexical_cast<std::string>(server_socket->remote_endpoint()),
        Logger::LOG_LEVEL::WARNING
    );
    Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::DEBUG);

    if (!error)
    {
        async_write(
            client_socket_,
            boost::asio::buffer(server_buffer_,bytes_transferred),
            boost::bind(
                &Bridge::handle_client_write,
                shared_from_this(),
                server_socket,
                boost::asio::placeholders::error
            )
        );
    }
}

void Bridge::handle_client_write(std::shared_ptr<socket_type> server_socket,
                                 const boost::system::error_code& error)
{
    std::cout << "handle_client_write" <<std::endl;

    if(error) 
    {
        std::cout << "6666666" <<std::endl;
        close(server_socket, Bridge::SOCKET_ERROR_SOURCE::CLIENT_ERROR); 
        return;
    }
    Logger::log(
        "Client <-- Proxy     Server.   [C] " + 
        boost::lexical_cast<std::string>(client_socket_.remote_endpoint()) + "  [S] " +
        boost::lexical_cast<std::string>(server_socket->remote_endpoint()),
        Logger::LOG_LEVEL::WARNING
    );
    Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::DEBUG);

    if (!error)
    {            
        server_socket->async_read_some(
            boost::asio::buffer(server_buffer_,max_data_length),
            boost::bind(
                &Bridge::handle_server_read,
                shared_from_this(),
                server_socket,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred
            )
        );
    }
}

void Bridge::close(std::shared_ptr<socket_type> server_socket,  
                   SOCKET_ERROR_SOURCE error_source)
{
    std::cout <<"()())()()()()()()()()" <<std::endl;
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;

    try
    {
        // If the error is in the clinet socket, close it as well as all server sockets
        if( error_source == Bridge::SOCKET_ERROR_SOURCE::CLIENT_ERROR )
        {
            // Logger::log(
            //     "Error with client socket, closing all bridge [C] " + 
            //     boost::lexical_cast<std::string>(client_socket_.remote_endpoint()),
            //     Logger::LOG_LEVEL::FATAL
            // );
            std::cout << "*****************************" << std::endl;
            std::cout << "*****************************" << std::endl;
            std::cout << "*****************************" << std::endl;
            if (client_socket_.is_open())
            {
                std::cout << "starting closing client" << std::endl;
                client_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
                // client_socket_.close();
                std::cout << "finished closing client" << std::endl;
            }
            std::cout << "starting closing all servers" << std::endl;
            for(const auto& server_socket_iter: server_socket_map_)
            {
                std::cout << "1 - ";
                std::cout << server_socket_iter.first << std::endl;
                server_socket_iter.second->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
                // server_socket_iter.second->close();
            }
            std::cout << "finished closing all servers" << std::endl;
            server_socket_map_.clear();
            std::cout << "finished clearing map" << std::endl;
        }

        // Logger::log(
        //     "Error with server socket [S] " + 
        //     boost::lexical_cast<std::string>(client_socket_.remote_endpoint()),
        //     Logger::LOG_LEVEL::FATAL
        // );
        
        // Error is with one of the server sockets
        else{
            if (server_socket->is_open())
                    {
                        std::cout << "size before del: " << server_socket_map_.size() << " server is open" << server_socket->is_open() << std::endl; 
                        server_socket_map_.erase(boost::lexical_cast<std::string>(server_socket->remote_endpoint()));
                        std::cout << "size after del: " << server_socket_map_.size() << std::endl; 
                        server_socket->shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
                        server_socket->close();
                    }
        }
    }
    catch( std::exception & ex )
    {
        Logger::log("Error during close",Logger::LOG_LEVEL::FATAL);
        Logger::log(ex.what(),Logger::LOG_LEVEL::FATAL);
    }

    
            
    // Logger::log("Current num of sockets - " + boost::lexical_cast<std::string>(Bridge::bridge_counter), Logger::LOG_LEVEL::DEBUG);
    

  // No new asynchronous operations are started. This means that all shared_ptr
  // references to the connection object will disappear and the object will be
  // destroyed automatically after this handler returns. The connection class's
  // destructor closes the socket.
}

Bridge::~Bridge(){
      Logger::log("d'tor of Bridge ", Logger::LOG_LEVEL::FATAL);
  }