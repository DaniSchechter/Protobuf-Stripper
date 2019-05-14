#ifndef UTILS_HPP_
#define UTILS_HPP_

#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>

#define ENDPOINT_ADDRESS_ERROR "0.0.0.0:0"
#define DEFAULT_HTTP_METHOD "Get "

// Parser for incoming requests.
struct Utils
{ 
    enum HTTP_MESSAGE_ERROR
    {
        EMPTY_DOMAIN = 1, EMPTY_ABSOLUTE_URI
    };
    
    Utils() = delete;

    // Get the host from http message
    static int parse_domain(const std::string& message, std::string& domain);

    // Resolve the host to endpoint
    static boost::asio::ip::tcp::endpoint resolve_endpoint(std::string domain,
                                                           boost::asio::io_context& io_context);

    // If the requested domain is empty (GET / http...) return a new request with the host as the domain
    static std::string generate_absolute_uri_request(const std::string& message, const std::string& http_type);

    static std::string split_domain(const std::string& domain);
};

#endif // UTILS_HPP_
