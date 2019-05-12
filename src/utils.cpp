#include "utils.hpp"

#include <regex>
#include <iostream>

std::string Utils::parse_domain(const std::string& message)
{
    std::regex re("^.+?[ \\t]+(?:https?|ftp:[/]{2})?([^/ :\\t\\n]+)(?:.*?:(\\d+))?");
    std::smatch match;
    std::regex_search(message, match, re);

    std::string port = "21";

    // If there is no valid URI
    if (!match[1].matched)
    {
        return EMPTY_DOMAIN;
    }

    std::string host = match[1].str(); 

    if(match[2].matched)
    {
        port = match[2].str();
    } 
    return host + ":" + port;
}

boost::asio::ip::tcp::endpoint Utils::resolve_endpoint(std::string domain,
                                                       boost::asio::io_context& io_context)
{
    // Resolve the end point for the requested domain
    int pos = domain.find_first_of(":");
    std::string host = domain.substr(0, pos);
    std::string port = domain.substr(pos+1);
    std::cout << host << " " << port << std::endl;
    try{
        // Resolve the absoute URI to ip and port - endpoint
        boost::asio::ip::tcp::resolver resolver( io_context );
        boost::asio::ip::tcp::resolver::query query(host, port);
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
        boost::asio::ip::tcp::endpoint endpoint = iterator->endpoint();
        return endpoint;
    }
    catch ( std::exception & ex )
    {
        return boost::asio::ip::tcp::endpoint();
    }
}
