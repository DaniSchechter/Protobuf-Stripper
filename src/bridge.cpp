#include "bridge.hpp"
#include "request_parser.hpp"

bridge::bridge(std::shared_ptr<boost::asio::io_context> io_context)
  : strand_(*io_context),
    io_context_(io_context),
    client_socket_(*io_context),
    server_socket_(*io_context){}

socket_type& bridge::client_socket()
{
  return client_socket_;
}

socket_type& bridge::server_socket()
{
  return server_socket_;
}

void bridge::start()
{

    // Read the first packet from the client and resolve its destination
    client_socket_.async_read_some(
        boost::asio::buffer(client_buffer_, max_data_length),
        boost::bind(
            &bridge::connect_after_read,
            shared_from_this(),
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred
        )
    );
}

// Hanadle first packet read from the client, resolve it's destination and continue in communication cycle
void bridge::connect_after_read(const boost::system::error_code& error,
                                std::size_t bytes_transferred)
{   

    // TODO handle errors
    if (error){}
    std::string host = request_parser::resolve_host(client_buffer_);
    boost::asio::ip::tcp::resolver resolver( *io_context_ );
    boost::asio::ip::tcp::resolver::query query(host, "80");
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
    boost::asio::ip::tcp::endpoint endpoint = iterator->endpoint();

    Logger::log("Attempting to connect to " + host + " on " +  boost::lexical_cast<std::string>(endpoint), Logger::LOG_LEVEL::WARNING);

    
    // Attempt connection to remote server (server side)
    server_socket_.async_connect(
        endpoint,
        boost::bind(&bridge::handle_server_connect,
            shared_from_this(),
            boost::asio::placeholders::error));

    async_write(
        server_socket_,
        boost::asio::buffer(client_buffer_,bytes_transferred),
        boost::bind(
            &bridge::handle_server_write,
            shared_from_this(),
            boost::asio::placeholders::error));

    Logger::log("Got from client, and sending to server:", Logger::LOG_LEVEL::WARNING);
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::INFO);
}

void bridge::handle_server_connect(const boost::system::error_code& error)
{
    if (!error)
    {
        // Setup async read from remote server (server)
        server_socket_.async_read_some(
                boost::asio::buffer(server_buffer_,max_data_length),
                boost::bind(&bridge::handle_server_read,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
    }
    else
        close();
    
    Logger::log("Connecting established", Logger::LOG_LEVEL::INFO);
}

// Read from client complete, now send data to remote server
void bridge::handle_client_read(const boost::system::error_code& error,
                                std::size_t bytes_transferred)
{
    Logger::log("Got from client, and sending to server:", Logger::LOG_LEVEL::WARNING);
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::INFO);
    if (!error)
    {
        async_write(server_socket_,
                boost::asio::buffer(client_buffer_,bytes_transferred),
                boost::bind(&bridge::handle_server_write,
                    shared_from_this(),
                    boost::asio::placeholders::error));
    }
    else
        bridge::close();
}


// Write to remote server complete, Async read from client
void bridge::handle_server_write(const boost::system::error_code& error)
{
    Logger::log("After writing to server", Logger::LOG_LEVEL::WARNING);
    Logger::log(std::string(client_buffer_), Logger::LOG_LEVEL::INFO);
    if (!error)
    {
        client_socket_.async_read_some(
            boost::asio::buffer(client_buffer_,max_data_length),
            boost::bind(&bridge::handle_client_read,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
    else {
        bridge::close();
    }
}

// Read from client complete, now send data to remote server
void bridge::handle_server_read(const boost::system::error_code& error,
                            const size_t& bytes_transferred)
{
    Logger::log("After reading from server:", Logger::LOG_LEVEL::WARNING);
    Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::INFO);
    if (!error)
    {
    async_write(client_socket_,
            boost::asio::buffer(server_buffer_,bytes_transferred),
            boost::bind(&bridge::handle_client_write,
                shared_from_this(),
                boost::asio::placeholders::error));
    }
    else
    close();
    
}

void bridge::handle_client_write(const boost::system::error_code& error)
{
    Logger::log("After sending to client:", Logger::LOG_LEVEL::WARNING);
    Logger::log(std::string(server_buffer_), Logger::LOG_LEVEL::INFO);
    if (!error)
    {            

    server_socket_.async_read_some(
            boost::asio::buffer(server_buffer_,max_data_length),
            boost::bind(&bridge::handle_server_read,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));


    }
    else
    bridge::close();
}

void bridge::close()
{
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    client_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);


  // No new asynchronous operations are started. This means that all shared_ptr
  // references to the connection object will disappear and the object will be
  // destroyed automatically after this handler returns. The connection class's
  // destructor closes the socket.
}