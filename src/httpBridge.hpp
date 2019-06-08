#ifndef HTTP_BRIDGE_HPP_
#define HTTP_BRIDGE_HPP_

#include "bridge.hpp"

class HttpBridge: public Bridge<HttpSocketType>
{
public:

    // Construct an Http bridge with a given client socket
    // On which the bridge connector accepted connection
    explicit HttpBridge(std::shared_ptr<boost::asio::io_context> io_context,
                        HttpSocketType& client_socket);

    /* Override functions */          
    std::shared_ptr<HttpSocketType> create_new_server_socket();

    virtual void save_first_message(char client_buffer [max_data_length]);
};

#endif // HTTP_BRIDGE_HPP_