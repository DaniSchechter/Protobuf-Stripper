#ifndef FTP_BRIDGE_HPP_
#define FTP_BRIDGE_HPP_

#define SECURE_UPGRADE_MESSAGE "234 Proceed with negotiation"

#include "bridge.hpp"
#include "ftpsBridge.hpp"

class FtpBridge: public Bridge<HttpSocketType>
{
public:

    // Construct an Http bridge with a given client socket
    // On which the bridge connector accepted connection
    explicit FtpBridge(std::shared_ptr<boost::asio::io_context> io_context,
                        HttpSocketType& client_socket,
                        const std::string& domain);

    /* Override functions */
    virtual void handle_server_read(std::shared_ptr<HttpSocketType> server_socket, 
                                    const boost::system::error_code& error,
                                    const size_t& bytes_transferred,
                                    const std::string& server_host);

    virtual std::shared_ptr<HttpSocketType> create_new_server_socket();

private:
    
    std::string domain_;

};

#endif // HTTP_BRIDGE_HPP_