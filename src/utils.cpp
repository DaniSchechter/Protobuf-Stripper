#include "utils.hpp"

#include <regex>
#include <iostream>

int Utils::parse_domain(const std::string& message, std::string& domain)
{
    std::regex re("^.+?[ \\t]+(ftp|https?)?(?:[:][/]{2})?([^/ :\\t\\n]+|/)(?:.*?:(\\d+))?.*?[ \\t]+");
    std::smatch match;
    std::regex_search(message, match, re);

    std::string port = "80";

    // If there is no valid URI
    if(!match[2].matched)
    {
        return EMPTY_DOMAIN;
    }

    // The absolute URI is /
    if(match[1].str()[0] == '/')
    {
        return EMPTY_ABSOLUTE_URI;
    }

    std::string host = match[2].str(); 

    if(match[3].matched)
    {
        port = match[3].str();
    } 
    else if(match[1].matched && match[1].str() == "ftp")
    {
        port = "21";
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
        http_method = DEFAULT_HTTP_METHOD;
    }
    return http_method + host[1].str() + "/" + full_message[2].str();
}

int Utils::fetch_common_name(const std::string& domain, std::string& common_name)
{
    /*  Last part is two characters long
        ------------------------------------------------------------------
        ([^.]+?[.][^.:]+[.:][^.]{2}) - something.something.(two characters)
        (?:[:]\\d+)?$ - if there is a port, e.g youtube.com:443, ignore the port till the end
    */

    /*  Two of more dots, when the last part is more than two characters long (not il, uk etc...)
        -----------------------------------------------------------------------------------------
        (?:[^.]+[.]) - for two or more dots, e.g www.youtube.org.il, ignore the forst subdomain (www)
        ((?:[^.:]+[.]?){3,}[^.:]+) - for two or more dots, e.g www.youtube.org.il, 
                                     capture everything from the first subdomain till the end
        (?:[:]\\d+)?$ - if there is a port, e.g youtube.com:443, ignore the port till the end
    */

    /*  E.g google.com
        -----------------------------------------------------------------------------------------
        ([^.]+[.][^.:]+) - capture: subdomain.domain
        (?:[:]\\d+)?$ - if there is a port, e.g youtube.com:443, ignore the port till the end
    */
        
    std::regex re("([^.]+?[.][^.:]+[.:][^.]{2})(?:[:]\\d+)?$|(?:[^.]+[.])((?:[^.:]+[.]?){3,}[^.:]+)(?:[:]\\d+)?$|([^.]+[.][^.:]+)(?:[:]\\d+)?$|((\\d+[.]){3}\\d+)[:]\\d+$");
    std::smatch match;
    std::regex_search(domain, match, re);

    for(int i=1; i<match.length(); i++)
    {
        if(match[i].matched)
        {
            common_name.assign(match[i].str(), 0, match[i].str().length());
            return 0;
        }

    } 
    
    return COMMON_NAME_ERROR;
}