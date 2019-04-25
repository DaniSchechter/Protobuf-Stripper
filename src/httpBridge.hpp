#ifndef HTTP_BRIDGE_HPP
#define HTTP_BRIDGE_HPP

#include "bridge.hpp"
#include <boost/asio/ssl.hpp>

class HttpBridge: public Bridge<HttpBridge, HttpSocketType>
{
public:

    // Construct an Http bridge with a given client socket
    // On which the bridge connector accepted connection
    explicit HttpBridge(std::shared_ptr<boost::asio::io_context> io_context,
                        HttpSocketType& client_socket);

    explicit HttpBridge(std::shared_ptr<boost::asio::io_context> io_context);

    /* Override functions */
    HttpSocketType& get_actual_socket(HttpSocketType& socket);
    std::shared_ptr<HttpSocketType> create_new_server_socket();
};

#endif // HTTP_BRIDGE_HPP