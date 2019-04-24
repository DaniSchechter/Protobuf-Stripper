#ifndef BRIDGE_HPP_
#define BRIDGE_HPP_

#include "logger.hpp"
#include "utils.hpp"

#include <unordered_map>
#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/ip/tcp.hpp>

typedef boost::asio::ip::tcp::endpoint endpoint_type;

template <typename SocketType>
// Represents a single connection from a client to a server
class Bridge
  : public std::enable_shared_from_this<Bridge<SocketType>>
{
public:
  
  enum class SOCKET_ERROR_SOURCE
  {
    CLIENT_READ_ERROR, CLIENT_WRITE_ERROR, SERVER_READ_ERROR, SERVER_WRITE_ERROR, SERVER_CONNECT_ERROR  
  };

  // Construct a connection with the given io_context.
  explicit Bridge(std::shared_ptr<boost::asio::io_context> io_context);

  // Get the socket associated with the client
  SocketType& client_socket();

  // Start the first asynchronous operation for the connection.
  void start();

  ~Bridge();

protected:

  // Handle completion of a client write operation.
  // Start a new server read operation
  void handle_client_write(std::shared_ptr<SocketType> server_socket, 
                          const boost::system::error_code& error, 
                          const std::string& endpoint);

  // Handle completion of a server read operation.
  // Start a new clinet write operation
  void handle_server_read(std::shared_ptr<SocketType> server_socket, 
                          const boost::system::error_code& error,
                          const size_t& bytes_transferred,
                          const std::string& endpoint);

  // Handle completion of a server write operation.
  // Start a clinet read operation
  void handle_server_write(std::shared_ptr<SocketType> server_socket, 
                           const boost::system::error_code& error,
                           const std::string& endpoint);

  // Handle the completion of the first client read operation.
  // Connect to the remote server
  void handle_client_read(const boost::system::error_code& error,
                          std::size_t bytes_transferred);
  
  // Handle completion of client read operation.
  // Start a server write operation
  void handle_client_read(std::shared_ptr<SocketType> server_socket,
                          const boost::system::error_code& error,
                          std::size_t bytes_transferred,
                          const std::string& endpoint);

  // Handle connection to a remote server.
  // Send the first received message from the client, to the remote servore
  // Invoke client read operation
  void handle_server_connect(std::shared_ptr<SocketType> server_socket, 
                             const boost::system::error_code& error,
                             std::size_t bytes_transferred,
                             const std::string& endpoint);

  // Close the connection with both client and server
  void close(std::shared_ptr<SocketType> server_socket, 
             SOCKET_ERROR_SOURCE error_source,
             const std::string& endpoint);

  void print_error_source(SOCKET_ERROR_SOURCE error_source);

  // Strand to ensure the connection's handlers are not called concurrently.
  boost::asio::io_context::strand strand_; // TODO check if needed and if not remove

  std::shared_ptr<boost::asio::io_context> io_context_;

  // Socket for the connection to the client
  SocketType client_socket_;

  // Client host and port
  std::string client_host_;

  // Map saving all server sockets for open server connections
  // Maps server's domain (<Host>:<Port>) to it's socket
  std::unordered_map< std::string, std::shared_ptr<SocketType> > server_socket_map_;
  
  // TODO switch to a generic bufffer like Dani the king saw in a video
  // TODO get this conf in conf file
  enum { max_data_length = 8192 }; //8KB
  char client_buffer_ [max_data_length];
  char server_buffer_  [max_data_length];
};

#endif //BRIDGE_HPP_
