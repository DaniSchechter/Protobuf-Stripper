#include "bridge.hpp"

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/write.hpp>

Bridge::Bridge(std::shared_ptr<boost::asio::io_context> io_context)
  : strand_(*io_context),
    io_context_(io_context),
    client_socket_(*io_context),
    server_socket_(*io_context){ 
    }

socket_type& Bridge::client_socket()
{
  return client_socket_;
}

socket_type& Bridge::server_socket()
{
  return server_socket_;
}

void Bridge::start()
{

    // Read the first packet from the client and resolve its destination
    client_socket_.async_read_some(
        boost::asio::buffer(client_buffer_, max_data_length),
        boost::bind(
            &Bridge::connect_after_read,
            shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

// Hanadle first packet read from the client, resolve it's destination and continue in communication cycle
void Bridge::connect_after_read(const boost::system::error_code& error,
                                std::size_t bytes_transferred)
{   

    Logger::log("Client --> Proxy     Server", Logger::LOG_LEVEL::WARNING);
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);

    // TODO handle errors
    if (error)
    {
        return;
    }

    // Resolve the absolute-uri from the first buffer got from client
    boost::asio::ip::tcp::endpoint endpoint = Request_parser::resolve_host(
        boost::lexical_cast<std::string>(client_buffer_),
        io_context_
    );
    
    Logger::log("Attempting to connect to "+ boost::lexical_cast<std::string>(endpoint), Logger::LOG_LEVEL::WARNING);

    
    // Attempt connection to remote server (server side)
    server_socket_.async_connect(
        endpoint,
        boost::bind(&Bridge::handle_server_connect,
            shared_from_this(),
            boost::asio::placeholders::error));

    async_write(
        server_socket_,
        boost::asio::buffer(client_buffer_,bytes_transferred),
        boost::bind(
            &Bridge::handle_server_write,
            shared_from_this(),
            boost::asio::placeholders::error));
}

void Bridge::handle_server_connect(const boost::system::error_code& error)
{
    if (!error)
    {
        // Setup async read from remote server (server)
        server_socket_.async_read_some(
                boost::asio::buffer(server_buffer_,max_data_length),
                boost::bind(&Bridge::handle_server_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    }
    else
        Bridge::close("From handle_server_connect");
    
    Logger::log("Connecting established", Logger::LOG_LEVEL::INFO);
}

// Read from client complete, now send data to remote server
void Bridge::handle_client_read(const boost::system::error_code& error,
                                std::size_t bytes_transferred)
{
    Logger::log("Client --> Proxy     Server", Logger::LOG_LEVEL::WARNING);
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);

    if (!error)
    {
        async_write(server_socket_,
                boost::asio::buffer(client_buffer_,bytes_transferred),
                boost::bind(&Bridge::handle_server_write,
                    shared_from_this(),
                    boost::asio::placeholders::error));
    }
    else
    {
        Bridge::close("From handle_client_read");
    }
}


// Write to remote server complete, Async read from client
void Bridge::handle_server_write(const boost::system::error_code& error)
{
    Logger::log("Client     Proxy --> Server", Logger::LOG_LEVEL::WARNING);
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::DEBUG);

    if (!error)
    {
        Logger::log("waiting to read from client_socket_", Logger::LOG_LEVEL::INFO);
        client_socket_.async_read_some(
            boost::asio::buffer(client_buffer_,max_data_length),
            boost::bind(&Bridge::handle_client_read,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
    else {
        Bridge::close("From handle_server_write");
    }
}

// Read from client complete, now send data to remote server
void Bridge::handle_server_read(const boost::system::error_code& error,
                            const size_t& bytes_transferred)
{
    Logger::log("Client     Proxy <-- Server", Logger::LOG_LEVEL::WARNING);
    Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::DEBUG);

    if (!error)
    {
    async_write(client_socket_,
            boost::asio::buffer(server_buffer_,bytes_transferred),
            boost::bind(&Bridge::handle_client_write,
                shared_from_this(),
                boost::asio::placeholders::error));
    }
    else {
        Bridge::close("From handle_server_read");
    }   
    
}

void Bridge::handle_client_write(const boost::system::error_code& error)
{
    Logger::log("Client <-- Proxy     Server", Logger::LOG_LEVEL::WARNING);
    Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::DEBUG);

    if (!error)
    {            
        Logger::log("waiting to read from server_socket_", Logger::LOG_LEVEL::INFO);
        server_socket_.async_read_some(
            boost::asio::buffer(server_buffer_,max_data_length),
            boost::bind(&Bridge::handle_server_read,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
    else {
        Bridge::close("From handle_client_write"); 
    }
    
}

void Bridge::close(const std::string & source)
{
    //TODO add mutex
    Logger::log("error: " + source, Logger::LOG_LEVEL::DEBUG);
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    if (client_socket_.is_open()){
        client_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        client_socket_.close();

    }
    if (server_socket_.is_open()){
        server_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);
        server_socket_.close();
    }
    return;
            
    // Logger::log("Current num of sockets - " + boost::lexical_cast<std::string>(Bridge::bridge_counter), Logger::LOG_LEVEL::DEBUG);
    

  // No new asynchronous operations are started. This means that all shared_ptr
  // references to the connection object will disappear and the object will be
  // destroyed automatically after this handler returns. The connection class's
  // destructor closes the socket.
}

Bridge::~Bridge(){
      Logger::log("d'tor of Bridge ", Logger::LOG_LEVEL::FATAL);
  }