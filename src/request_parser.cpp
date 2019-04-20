#include "request_parser.hpp"

#include <regex>
#include <iostream>

boost::asio::ip::tcp::endpoint Request_parser::resolve_host(const std::string& message, std::shared_ptr<boost::asio::io_context> io_context)
{
    // Fetch the URI ( host:port )
    std::regex re("^.+?[ \\t]+(?:https?:[/]{2})([^/ \\t\\n]+)(?:.*?:(\\d+))?");
    std::smatch match;
    std::regex_search(message, match, re);

    std::string port = "80";

    // TODO add error checking
    std::string host = match[1].str(); 
    // If there is no valid URI
    if (!match[1].matched)
    {
        return boost::asio::ip::tcp::endpoint();
    }

    if(match[2].matched)
    {
        port = match[2].str();
    } 


    // Resolve the absoute URI to ip and port - endpoint
    boost::asio::ip::tcp::resolver resolver( *io_context );
    boost::asio::ip::tcp::resolver::query query(host, port);
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
    boost::asio::ip::tcp::endpoint endpoint = iterator->endpoint();

    return endpoint;
}
