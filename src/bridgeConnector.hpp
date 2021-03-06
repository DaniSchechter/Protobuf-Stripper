#ifndef BRIDGE_CONNECTOR_HPP_
#define BRIDGE_CONNECTOR_HPP_

#include "httpBridge.hpp"
#include "httpsBridge.hpp"
#include "ftpBridge.hpp"
#include "ftpsBridge.hpp"

class BridgeConnector
    : public std::enable_shared_from_this<BridgeConnector>
{
public:

    enum KNOWN_PORTS
    {
        FTP = 21, HTTP = 80, HTTPS = 443
    };

    explicit BridgeConnector(std::shared_ptr<boost::asio::io_context> io_context);

    // Returns the client socket on which the accept was done
    // This will also be used by the bridge itself later on 
    boost::asio::ip::tcp::socket& client_socket() { return client_socket_; }

    // Reads the first message from the client
    void start();

    // Handle the completion of the first client read operation.
    // Initializes the correct Bridge (http / https) to hande the request
    void handle_client_read(const boost::system::error_code& error,
                            std::size_t bytes_transferred);    

private:

    void handle_connect_request(const std::string& client_remote_endpoint);

    std::shared_ptr<boost::asio::io_context> io_context_;
    boost::asio::ip::tcp::socket client_socket_;

    enum { max_data_length = 8192 }; //8KB
    char client_buffer_ [max_data_length];
};

#endif // BRIDGE_CONNECTOR_HPP_