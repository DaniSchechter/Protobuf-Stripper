#ifndef BRIDGE_HPP_
#define BRIDGE_HPP_

#include "logger.hpp"
#include "request_parser.hpp"

#include <map>
#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/ip/tcp.hpp>

typedef boost::asio::ip::tcp::socket socket_type;

// Represents a single connection from a client to a server
class Bridge
  : public std::enable_shared_from_this<Bridge>
{
public:
  
  enum class SOCKET_ERROR_SOURCE
  {
    CLIENT_ERROR, SERVER_ERROR
  };

  // Construct a connection with the given io_context.
  explicit Bridge(std::shared_ptr<boost::asio::io_context> io_context);

  // Get the socket associated with the client
  socket_type& client_socket();

  // Get the socket associated with the server
  socket_type& server_socket();

  // Start the first asynchronous operation for the connection.
  void start();

  ~Bridge();

private:

  // Handle completion of a client write operation.
  // Start a new server read operation
  void handle_client_write(std::shared_ptr<socket_type> server_socket, const boost::system::error_code& error);

  // Handle completion of a server read operation.
  // Start a new clinet write operation
  void handle_server_read(std::shared_ptr<socket_type> server_socket, 
                          const boost::system::error_code& error,
                          const size_t& bytes_transferred);

  // Handle completion of a server write operation.
  // Start a clinet read operation
  void handle_server_write(std::shared_ptr<socket_type> server_socket, 
                           const boost::system::error_code& error);

  // Handle completion of a client read operation.
  // Start a server write operation
  void handle_client_read(const boost::system::error_code& error,
                          std::size_t bytes_transferred);
                          
  void handle_client_read(std::shared_ptr<socket_type> server_socket,
                          const boost::system::error_code& error,
                          std::size_t bytes_transferred);

  // Handle connection to a remote server.
  void handle_server_connect(std::shared_ptr<socket_type> server_socket, 
                             const boost::system::error_code& error,
                             std::size_t bytes_transferred);

  // Close the connection with both client and server
  void close(std::shared_ptr<socket_type> server_socket, 
             SOCKET_ERROR_SOURCE error_source);

  // Strand to ensure the connection's handlers are not called concurrently.
  boost::asio::io_context::strand strand_; // TODO check if needed and if not remove

  std::shared_ptr<boost::asio::io_context> io_context_;

  // Socket for the connection to the client
  socket_type client_socket_;

  // Map saving all server sockets for open server connections
  // Maps server's host (and not IP) to it's socket
  std::map<const std::string, std::shared_ptr<socket_type >> server_socket_map_;
  
  // TODO switch to a generic bufffer like Dani the king saw in a video
  // TODO get this conf in conf file
  enum { max_data_length = 8192 }; //8KB
  char client_buffer_ [max_data_length];
  char server_buffer_  [max_data_length];
  
};

#endif //BRIDGE_HPP_
