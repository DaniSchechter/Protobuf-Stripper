#ifndef SECURE_BRIDGE_HPP
#define SECURE_BRIDGE_HPP

#include "bridge.hpp"
#include <boost/asio/ssl.hpp>
#include <boost/asio/ssl/context.hpp>

typedef boost::asio::ssl::stream<boost::asio::ip::tcp::socket> ssl_stream_type;

class SecureBridge: private Bridge<ssl_stream_type>
{
public:

    explicit SecureBridge(std::shared_ptr<boost::asio::io_context> io_context,
                          boost::asio::ssl::context& ssl_context);

private:

    ssl_stream_type ssl_stream_;    
};

#endif // SECURE_BRIDGE_HPP