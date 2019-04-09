#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

/// Represents a single connection from a client.
class connection
  : public boost::enable_shared_from_this<connection>
{
public:

  typedef boost::asio::ip::tcp::socket socket_type;

  /// Construct a connection with the given io_service.
  explicit connection(std::shared_ptr<boost::asio::io_service> io_service);

  /// Get the socket associated with the connection.
  socket_type& downstream_socket();

  socket_type& upstream_socket();

  enum { max_data_length = 8192 }; //8KB
  unsigned char downstream_data_[max_data_length];
  unsigned char upstream_data_  [max_data_length];

  /// Start the first asynchronous operation for the connection.
  void start();

  

private:
  /// Handle completion of a read operation.
  void handle_downstream_write(const boost::system::error_code& error);

  void handle_upstream_read(const boost::system::error_code& error,
                            const size_t& bytes_transferred);

  void handle_upstream_write(const boost::system::error_code& error);

  void handle_downstream_read(const boost::system::error_code& error,
    std::size_t bytes_transferred);

  void connect_to_server(const std::string downstream_data);
  

  void close();

  /// Strand to ensure the connection's handlers are not called concurrently.
  boost::asio::io_service::strand strand_;

  // Socket for the connection to the client
  socket_type downstream_socket_;
    

  // Socket for the connection to the server
  socket_type upstream_socket_;

  std::shared_ptr<boost::asio::io_service> io_service;

  /// Buffer for incoming data.
//   boost::array<char, 8192> buffer_;
  
  enum { max_data_length = 8192 }; //8KB
  unsigned char downstream_data_ [max_data_length];
  unsigned char upstream_data_  [max_data_length];
  

};

typedef boost::shared_ptr<connection> connection_ptr;
