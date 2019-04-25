#ifndef HTTP_BRIDGE_HPP
#define HTTP_BRIDGE_HPP


#include "bridge.hpp"
#include <boost/asio/ssl.hpp>

class HttpBridge: public Bridge<HttpSocketType>
{
public:

    // Construct a connection with the given io_context.
    explicit HttpBridge(std::shared_ptr<boost::asio::io_context> io_context,
                        HttpSocketType& socket);

    HttpSocketType client_socket_;
};

#endif // HTTP_BRIDGE_HPP