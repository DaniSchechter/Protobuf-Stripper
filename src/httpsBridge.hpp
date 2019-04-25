#ifndef HTTPS_BRIDGE_HPP
#define HTTPS_BRIDGE_HPP

#include "bridge.hpp"
#include <boost/asio/ssl/stream.hpp>
#include <boost/asio/ssl/context.hpp>

using SslStreamType = boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ;
using BasicSocketType = boost::asio::basic_socket<boost::asio::ip::tcp>;

class HttpsBridge: public Bridge<SslStreamType>
{
public:

    explicit HttpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                          HttpSocketType& socket);
                
private:

    boost::asio::ssl::context ctx_;
    SslStreamType ssl_stream_;    
};

#endif // HTTPS_BRIDGE_HPP