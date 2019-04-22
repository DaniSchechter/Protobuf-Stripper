#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#define ENDPOINT_ADDRESS_ERROR "0.0.0.0:0"
#define EMPTY_DOMAIN "1"

// Parser for incoming requests.
struct Utils
{ 
    
    Utils() = delete;

    // Get the host from http message
    static std::string parse_domain(const std::string& message);

    // Resolve the host to endpoint
    static boost::asio::ip::tcp::endpoint resolve_endpoint(std::string domain,
                                                           std::shared_ptr<boost::asio::io_context> io_context);
};

#endif // UTILS_HPP_
