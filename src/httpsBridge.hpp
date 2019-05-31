#ifndef HTTPS_BRIDGE_HPP_
#define HTTPS_BRIDGE_HPP_

#include "bridge.hpp"
#include <boost/asio/ssl/context.hpp>

class HttpsBridge: public Bridge<SslStreamType>
{
public:

    explicit HttpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                         HttpSocketType& client_socket,
                         boost::asio::ssl::context context);
    
    /* Override functions */
    
    // Start to handle the ssl handshake
    virtual void do_handshake(std::shared_ptr<SslStreamType> socket,
                              boost::asio::ssl::stream_base::handshake_type handshake_type);

    virtual std::shared_ptr<SslStreamType> create_new_server_socket();

    // Enables secure bridges to perform handshake
    virtual void close_socket(std::shared_ptr<SslStreamType> socket);

private:

    boost::asio::ssl::context ctx_;  
};

#endif // HTTPS_BRIDGE_HPP_