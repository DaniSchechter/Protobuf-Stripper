#include "utils.hpp"

#include <regex>
#include <iostream>

int Utils::parse_domain(const std::string& message, std::string& domain)
{
    std::regex re("^.+?[ \\t]+(?:https?:[/]{2})?([^/ :\\t\\n]+|/)(?:.*?:(\\d+))?.*?[ \\t]+");
    std::smatch match;
    std::regex_search(message, match, re);

    std::string port = "80";

    // If there is no valid URI
    if(!match[1].matched)
    {
        return EMPTY_DOMAIN;
    }

    // The absolute URI is /
    if(match[1].str()[0] == '/')
    {
        return EMPTY_ABSOLUTE_URI;
    }

    std::string host = match[1].str(); 

    if(match[2].matched)
    {
        port = match[2].str();
    } 

    std::string new_domain = host + ":" + port;

    domain.assign(new_domain, 0, new_domain.length());
    return 0;
}

boost::asio::ip::tcp::endpoint Utils::resolve_endpoint(std::string domain,
                                                       boost::asio::io_context& io_context)
{
    // Resolve the end point for the requested domain
    int pos = domain.find_first_of(":");
    std::string host = domain.substr(0, pos);
    std::string port = domain.substr(pos+1);

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

std::string Utils::generate_absolute_uri_request(const std::string& message, const std::string& http_type)
{
    // Get the host
    std::regex host_re("Host:[ \\t](.*)");
    std::smatch host;
    std::regex_search(message, host, host_re);
    
    // Get the rest of the request 
    // 1) Http method
    // 2) all the content after the /
    std::regex full_message_re("(.*?)(?:https?:/{2})?[/]([\\s\\S]+)");
    std::smatch full_message;
    std::regex_search(message, full_message, full_message_re);
    std::string http_method = full_message[1].str() + http_type + "://";

    // If the request starts with / and no HTTP Method, aconcat defult http method at the beginning 
    if(http_method == "")
    {
        std::cout << "*********************************************************************************************************\n";
        http_method = DEFAULT_HTTP_METHOD;
    }
    return http_method + host[1].str() + "/" + full_message[2].str();
}

