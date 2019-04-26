#ifndef HTTPS_BRIDGE_HPP
#define HTTPS_BRIDGE_HPP

#include "bridge.hpp"
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl/context.hpp>

using SslStreamType = boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ;
using BasicSocketType = boost::asio::basic_socket<boost::asio::ip::tcp>;

class HttpsBridge: public Bridge<HttpsBridge, SslStreamType>
{
public:

    explicit HttpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                         HttpSocketType& client_socket);

    // Start to handle the request
    // Connects to the requested remote server, and forwards the message it got from bridge connector
    void start_by_connect(char client_buffer [max_data_length],
                          const boost::system::error_code& error,
                          std::size_t bytes_transferred,
                          endpoint_type endpoint,
                          const std::string& domain);

    void do_client_ssl_handshake(const boost::system::error_code& error,
                                 size_t bytes_transferred,
                                 endpoint_type endpoint,
                                 const std::string& domain);
    
    /* Override functions */
    
    // Start to handle the ssl handshake
    void do_handshake(std::shared_ptr<SslStreamType> socket,
                      boost::asio::ssl::stream_base::handshake_type handshake_type);

    BasicSocketType& get_actual_socket(SslStreamType& socket);
    std::shared_ptr<SslStreamType> create_new_server_socket();

private:

    boost::asio::ssl::context ctx_;
    std::shared_ptr<SslStreamType> ssl_stream_;    
};

#endif // HTTPS_BRIDGE_HPP