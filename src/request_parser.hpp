#ifndef REQUEST_PARSER_HPP_
#define REQUEST_PARSER_HPP_

#include <string>
#include <iostream>
#include <utility>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>

// Parser for incoming requests.
struct Request_parser
{ 
    
    Request_parser() = delete;

    // Get the host from http message
    // TODO - create a generic http class representaition
    static boost::asio::ip::tcp::endpoint resolve_host(const std::string& message, std::shared_ptr<boost::asio::io_context> io_context);

};

#endif // REQUEST_PARSER_HPP_
