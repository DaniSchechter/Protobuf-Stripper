#include "request_parser.hpp"

std::string request_parser::resolve_host(const std::string& message)
{
    // boost::regex pattern("Host: ([^\r\n]+)\r\n", boost::regex_constants::icase);
    // boost::smatch match;
    // boost::regex_search(message, match, pattern);
    // return match[0].str();
    return "google.com";
}
