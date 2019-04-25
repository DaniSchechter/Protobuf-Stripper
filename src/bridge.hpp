#ifndef BRIDGE_HPP_
#define BRIDGE_HPP_

#include "logger.hpp"
#include "utils.hpp"

#include <unordered_map>
#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/ip/tcp.hpp>

#include <boost/asio/placeholders.hpp>
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio/write.hpp>

using HttpSocketType = boost::asio::ip::tcp::socket;
using endpoint_type = boost::asio::ip::tcp::endpoint; // TODO change to using in the whole project

// Represents a single connection from a client to a server
template <typename SocketType>
class Bridge
  : public std::enable_shared_from_this<Bridge<SocketType>>
{
private:

  enum { max_data_length = 8192 }; //8KB

public:
  
  enum class SOCKET_ERROR_SOURCE
  {
    CLIENT_READ_ERROR, CLIENT_WRITE_ERROR, SERVER_READ_ERROR, SERVER_WRITE_ERROR, SERVER_CONNECT_ERROR  
  };

  // Construct a connection with the given io_context.
  explicit Bridge(std::shared_ptr<boost::asio::io_context> io_context);

  // Start to handle the request
  // Connects to the requested remote server, and forwards the message it got from bridge connector
  void start_by_connect(char client_buffer [max_data_length],
                        endpoint_type endpoint,
                        const std::string& domain);

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

  // Overrided For each bridge type
  // Returns the correct socket type that supports the function calls
  SocketType& get_actual_socket();

  void print_error_source(SOCKET_ERROR_SOURCE error_source);

private:

  // Get the actual socket for which async operations are done
  // BridgeType* get_bridge_type() { return static_cast<BridgeType*>(this); }
  // BridgeType const* get_const_bridge_type() { return static_cast<BridgeType const*>(this); }


  // Strand to ensure the connection's handlers are not called concurrently.
  boost::asio::io_context::strand strand_; // TODO check if needed and if not remove

  std::shared_ptr<boost::asio::io_context> io_context_;

  std::shared_ptr<SocketType> client_socket_;

  // Client host and port
  std::string client_host_;

  // Map saving all server sockets for open server connections
  // Maps server's domain (<Host>:<Port>) to it's socket
  std::unordered_map< std::string, std::shared_ptr<SocketType> > server_socket_map_;
  
  // TODO switch to a generic bufffer like Dani the king saw in a video
  // TODO get this conf in conf file
  char client_buffer_ [max_data_length];
  char server_buffer_  [max_data_length];

protected:

  void set_client_socket(std::shared_ptr<SocketType> socket)
  {
    client_socket_ = socket;
  }

};

#endif //BRIDGE_HPP_
