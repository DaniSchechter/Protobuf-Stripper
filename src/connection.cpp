#include "connection.hpp"
#include <vector>
#include <boost/bind.hpp>
#include "connection_handler.hpp"
#include <iostream>

connection::connection(std::shared_ptr<boost::asio::io_service> io_service)
  : strand_(*io_service),
    downstream_socket_(*io_service),
    upstream_socket_(*io_service),
    io_service(io_service)
{
}

// connection::socket_type downstream_socket()
// {
//   return downstream_socket_;
// }

// socket_type& upstream_socket()
// {
//   return upstream_socket_;
// }


void connection::start()
{
  boost::bind(handle_downstream_read,
                    //   shared_from_this(),
                      boost::asio::placeholders::error,
                      boost::asio::placeholders::bytes_transferred)();
}


void something(){
      // std::cout << "after we got connect ,we sent back to client:\n" << downstream_data_ << std::endl;
      // std::cout << "-----------------------------------------------" <<std::endl;   
     }


// Read from client complete, now send data to remote server
void connection::handle_downstream_read(const boost::system::error_code& error,
    std::size_t bytes_transferred)
{
    if (!error)
    {
        std::string str = "HTTP/1.1 200 connection established\r\n";
        if(std::strstr((char *)downstream_data_,(char *)"CONNECT")){
            connect_to_server((char *)downstream_data_);
            std::copy(str.begin(), str.end() +1, downstream_data_);
            async_write(
                downstream_socket_,
                boost::asio::buffer(downstream_data_,bytes_transferred),
                boost::bind(something,
                shared_from_this()));
                
        }
        else
        {
        async_write(upstream_socket_,
                boost::asio::buffer(downstream_data_,bytes_transferred),
                boost::bind(&connection::handle_upstream_write,
                    shared_from_this(),
                    boost::asio::placeholders::error));}
    }
    else
        connection::close();
  }

  // If an error occurs then no new asynchronous operations are started. This
  // means that all shared_ptr references to the connection object will
  // disappear and the object will be destroyed automatically after this
  // handler returns. The connection class's destructor closes the socket.


void connection::connect_to_server(const std::string downstream_data){
    size_t host_index = downstream_data.find("Host:");
    char * host;
    for(host_index; downstream_data[host_index] != '\r';host_index++){
        host += downstream_data[host_index];
    }
    boost::asio::ip::tcp::resolver resolver( *io_service );
    boost::asio::ip::tcp::resolver::query query(host, "https");
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
    boost::asio::ip::tcp::endpoint endpoint = *iterator;

    upstream_socket_.connect(endpoint);
   }


// Write to remote server complete, Async read from client
void connection::handle_upstream_write(const boost::system::error_code& error)
{
    std::cout << "after we write to server: \n" << downstream_data_ << std::endl;
    std::cout << "-----------------------s-----------------------" <<std::endl;
    if (!error)
    {
        downstream_socket_.async_read_some(
            boost::asio::buffer(downstream_data_,max_data_length),
            boost::bind(&connection::handle_downstream_read,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }
    else {
        connection::close();
    }
}

// Read from client complete, now send data to remote server
void connection::handle_upstream_read(const boost::system::error_code& error,
                            const size_t& bytes_transferred)
{
    std::cout << "after we read from server: \n" << upstream_data_ << std::endl;
    std::cout << "after we read from server length: \n" << strlen((char *)upstream_data_) << std::endl;
    std::cout << "----------------------------------------------" <<std::endl;
    if (!error)
    {
    async_write(downstream_socket_,
            boost::asio::buffer(upstream_data_,bytes_transferred),
            boost::bind(&connection::handle_downstream_write,
                shared_from_this(),
                boost::asio::placeholders::error));
    }
    else
    close();
    
}

void connection::handle_downstream_write(const boost::system::error_code& error)
{
    std::cout << "after we sent to client: \n" << upstream_data_ << std::endl;
    std::cout << "----------------------------------------------" <<std::endl;
    if (!error)
    {            

    upstream_socket_.async_read_some(
            boost::asio::buffer(upstream_data_,max_data_length),
            boost::bind(&connection::handle_upstream_read,
                shared_from_this(),
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));


    }
    else
    connection::close();
}

void connection::close()
{
    // Initiate graceful connection closure.
    boost::system::error_code ignored_ec;
    downstream_socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both, ignored_ec);


  // No new asynchronous operations are started. This means that all shared_ptr
  // references to the connection object will disappear and the object will be
  // destroyed automatically after this handler returns. The connection class's
  // destructor closes the socket.
}