#ifndef FTP_BRIDGE_HPP
#define FTP_BRIDGE_HPP

#include "bridge.hpp"
#include <boost/asio/ssl.hpp>
#include <regex>
#include <boost/regex.hpp>
#include <boost/asio/read_until.hpp>
#include <boost/asio/streambuf.hpp>
class FtpBridge: public Bridge<FtpBridge, HttpSocketType>
{
public:

    // Construct an FTP bridge with a given client socket
    // On which the bridge connector accepted connection
    explicit FtpBridge(std::shared_ptr<boost::asio::io_context> io_context, HttpSocketType& client_socket);

    explicit FtpBridge(std::shared_ptr<boost::asio::io_context> io_context);

    // Start to handle the request
    // Connects to the requested remote server, and forwards the message it got from bridge connector
    void start_by_connect(char client_buffer [max_data_length],
                          const boost::system::error_code& error,
                          std::size_t bytes_transferred,
                          endpoint_type endpoint,
                          const std::string& domain);

    /* Override functions */
    void do_handshake(std::shared_ptr<HttpSocketType> socket, boost::asio::ssl::stream_base::handshake_type handshake_type);

    HttpSocketType& get_actual_socket(HttpSocketType& socket);
    std::shared_ptr<HttpSocketType> create_new_server_socket();
};

#endif // FTP_BRIDGE_HPP