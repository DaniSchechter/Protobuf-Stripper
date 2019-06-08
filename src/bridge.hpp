#ifndef BRIDGE_HPP_
#define BRIDGE_HPP_

#include "logger.hpp"
#include "utils.hpp"
#include "messageDecoder.hpp"

#include <unordered_map>
#include <boost/asio/io_context_strand.hpp>
#include <boost/asio/ssl.hpp>

using HttpSocketType = boost::asio::ip::tcp::socket;
using SslStreamType = boost::asio::ssl::stream<boost::asio::ip::tcp::socket>;

using endpoint_type = boost::asio::ip::tcp::endpoint; 

// Represents a single connection from a client to a server
template <typename SocketType>
class Bridge
  : public std::enable_shared_from_this<Bridge<SocketType>>
{  

public:
  
  enum class SOCKET_ERROR_SOURCE
  {
    CLIENT_READ_ERROR, CLIENT_WRITE_ERROR, SERVER_READ_ERROR, SERVER_WRITE_ERROR, SERVER_CONNECT_ERROR, BRIDGE_UPGRADED
  };

  enum { max_data_length = 32768 }; //8KB

  // Constructs a connection with the given io_context.
  explicit Bridge(std::shared_ptr<boost::asio::io_context> io_context);

  // Start to handle the request
  // Connects to the requested remote server, and forwards the message it got from bridge connector
  virtual void start_by_connect(char client_buffer [max_data_length],
                                const boost::system::error_code& error,
                                std::size_t bytes_transferred,
                                endpoint_type endpoint,
                                const std::string& domain);

  void handle_server_connect(std::shared_ptr<SocketType> server_socket,
                             const boost::system::error_code& error,
                             const std::string& server_host);

  // Handle completion of a client write operation.
  // Start a new server read operation
  void handle_client_write(std::shared_ptr<SocketType> server_socket, 
                           const boost::system::error_code& error, 
                           const std::string& server_host);

  // Close the connection with both client and server
  void close(std::shared_ptr<SocketType> server_socket, 
             SOCKET_ERROR_SOURCE error_source,
             const std::string& server_host,
             const boost::system::error_code& error);

protected:

  // Handle completion of a server read operation.
  // Start a new clinet write operation
  virtual void handle_server_read(std::shared_ptr<SocketType> server_socket, 
                                  const boost::system::error_code& error,
                                  const size_t& bytes_transferred,
                                  const std::string& server_host);

  // Creates and returns a new server socket
  virtual std::shared_ptr<SocketType> create_new_server_socket() = 0;

  // Enables secure bridges to perform handshake
  virtual void do_handshake(std::shared_ptr<SocketType> socket,
                            boost::asio::ssl::stream_base::handshake_type handshake_type);

  // Enables secure bridges to perform handshake
  virtual void close_socket(std::shared_ptr<SocketType> socket);
  
  // Enables derrived bridges to forward the first message from from client or drop it
  virtual void save_first_message(char client_buffer [max_data_length]);

  // Enable derived Bridges to set the correct socket
  void set_client_socket(std::shared_ptr<SocketType> socket) { client_socket_ = socket;}

  // Derived classes need this to create new server sockets
  std::shared_ptr<boost::asio::io_context> io_context_;

  // Client host and port
  std::string client_host_;

  // Buffers for saving messages 
  char client_buffer_ [max_data_length];
  char server_buffer_ [max_data_length];

  // Client socket for each socket that the browser creates
  std::shared_ptr<SocketType> client_socket_;

  // Strand to ensure the connection's handlers are not called concurrently.
  boost::asio::io_context::strand strand_;
  //class to decode protobuf messages
  MessageDecoder md_;

  // Map saving all server sockets for open server connections
  // Maps server's domain (<Host>:<Port>) to it's socket
  std::unordered_map< std::string, std::shared_ptr<SocketType> > server_socket_map_;

private:

  // Handle completion of a server write operation.
  // Start a clinet read operation
  void handle_server_write(std::shared_ptr<SocketType> server_socket, 
                           const boost::system::error_code& error,
                           const std::string& server_host);
  
  // Handle completion of client read operation.
  // Start a server write operation
  void handle_client_read(std::shared_ptr<SocketType> server_socket,
                          const boost::system::error_code& error,
                          std::size_t bytes_transferred,
                          const std::string& server_host);

  // Prints the error messaage indside close function
  void print_error_source(SOCKET_ERROR_SOURCE error_source, 
                          const std::string& server_host,
                          const boost::system::error_code& error);

};

#endif //BRIDGE_HPP_
