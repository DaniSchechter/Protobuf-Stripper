#ifndef FTPS_BRIDGE_HPP_
#define FTPS_BRIDGE_HPP_

#define DATA_CONNECTION_PORT_MESSAGE "229 Entering Extended Passive Mode"
#define ERROR "error"

#include "bridge.hpp"
#include <boost/asio/ssl/context.hpp>

class FtpsBridge: public Bridge<SslStreamType>
{
public:

    // C'tor for Connection chanel ( Port 21 )
    explicit FtpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                        std::shared_ptr<HttpSocketType> unsecure_client_socket,
                        std::shared_ptr<HttpSocketType> unsecure_server_socket,
                        boost::asio::ssl::context context,
                        const std::string& unsecure_server_host);

    /* C'tor for Data chanel ( Random Port )
     * @param io_context             - initialize the base class Bridge with it
     * @param unsecure_client_socket - the socket on which the server accepted connection
     * @param endpoint               - the endpoint the server socket should connect to ( on the random port )
     * @param context                - new context to initialize the SSL stream wrapper for client socket
     * @param unsecure_server_host   - save the server socket into server_socket_map_ 
     */
    explicit FtpsBridge(std::shared_ptr<boost::asio::io_context> io_context,
                        std::shared_ptr<HttpSocketType> unsecure_client_socket,
                        endpoint_type endpoint,
                        boost::asio::ssl::context context,
                        const std::string& unsecure_server_host);

    // Performs handshake with both sides and starts the event loop
    void run(const std::string& unsecure_server_host);

    /* Override functions */
    virtual void handle_server_read(std::shared_ptr<SslStreamType> server_socket, 
                                    const boost::system::error_code& error,
                                    const size_t& bytes_transferred,
                                    const std::string& server_host);
                                  
    virtual std::shared_ptr<SslStreamType> create_new_server_socket();

    // Enables secure bridges to perform handshake
    virtual void close_socket(std::shared_ptr<SslStreamType> socket);

    // Maps < data channel port, SSL_SESSION* > for session reuse 
    static std::unordered_map<std::string, SSL_SESSION*> session_cache_;

private:

    // Fetches the optional port number list that the client can choose from to establish data connection channel
    // E.g: "229 Entering Extended Passive Mode (|||47084|)" - fetches 47084
    bool fetch_data_channel_ports(const std::string& message, std::string& port);

    // Guards session_cache_
    static std::mutex session_cache_lock_; 

    std::shared_ptr<SslStreamType> secure_server_socket_;

    boost::asio::ssl::context ctx_;
};

#endif // FTPS_BRIDGE_HPP_