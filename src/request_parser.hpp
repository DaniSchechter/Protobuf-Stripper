#ifndef REQUEST_PARSER_HPP_
#define REQUEST_PARSER_HPP_

#include <string>
#include <iostream>
#include <boost/regex.hpp>

// Parser for incoming requests.
struct request_parser
{
    // request_parser() = delete;

    // Get the host from http message
    // TODO - create a generic http class representaition
    static std::string resolve_host(const std::string& message);

};

#endif // REQUEST_PARSER_HPP_
