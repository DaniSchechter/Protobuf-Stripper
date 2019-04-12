#include "bridge.hpp"
#include "request_parser.hpp"
#include <vector>
#include <boost/bind.hpp>
#include <iostream>

bridge::bridge(boost::shared_ptr<boost::asio::io_context> io_context)
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
    std::cout << "after get from client: " << client_buffer_ << std::endl;
    std::cout << "-----------------------------------------------" <<std::endl;

    // TODO handle errors
    if (error){}
    std::string host = request_parser::resolve_host(client_buffer_);
    boost::asio::ip::tcp::resolver resolver( *io_context_ );
    boost::asio::ip::tcp::resolver::query query(host, "80");
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
    boost::asio::ip::tcp::endpoint endpoint = iterator->endpoint();

    std::cout<< "the endpoint is: " << endpoint << std::endl;

    
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
}

// Read from client complete, now send data to remote server
void bridge::handle_client_read(const boost::system::error_code& error,
                                std::size_t bytes_transferred)
{
    std::cout << "after get from client: " << client_buffer_ << std::endl;
    std::cout << "-----------------------------------------------" <<std::endl;
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
    std::cout << "after we write to server: \n" << client_buffer_ << std::endl;
    std::cout << "-----------------------s-----------------------" <<std::endl;
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
    std::cout << "after we read from server: \n" << server_buffer_ << std::endl;
    std::cout << "after we read from server length: \n" << strlen((char *)server_buffer_) << std::endl;
    std::cout << "----------------------------------------------" <<std::endl;
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
    std::cout << "after we sent to client: \n" << server_buffer_ << std::endl;
    std::cout << "----------------------------------------------" <<std::endl;
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