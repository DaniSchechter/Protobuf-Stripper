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

    /* Override functions */
    BasicSocketType& get_actual_socket(SslStreamType& socket);
    std::shared_ptr<SslStreamType> create_new_server_socket();

private:

    boost::asio::ssl::context ctx_;
    std::shared_ptr<SslStreamType> ssl_stream_;    
};

#endif // HTTPS_BRIDGE_HPP