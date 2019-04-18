#include "request_parser.hpp"

#include <regex>


boost::asio::ip::tcp::endpoint Request_parser::resolve_host(const std::string& message, std::shared_ptr<boost::asio::io_context> io_context)
{
    // Fetch the URI ( host:port )
    std::regex re("^.+?[ \\t]+(?:https?:[/]{2})?(.*?):?(\\d+)?[/ \\t]");
    std::smatch match;
    std::regex_search(message, match, re);

    std::string host = match[1].str();
    std::string port = "80";
    
    if(match.size() == 3)
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
