#include "config.hpp"
#include "logger.hpp"

#include <boost/property_tree/json_parser.hpp>

Config::Config(const std::string& file_path)
{

    try 
    {
        boost::property_tree::ptree root_json_object;
        boost::property_tree::read_json(file_path, root_json_object);

        boost::property_tree::ptree http_proxy_json_object = root_json_object.get_child("httpProxy");

        // Http Proxy Configurations
        local_host_     = http_proxy_json_object.get<std::string>("localHost", "127.0.0.1");
        local_port_     = http_proxy_json_object.get<int>        ("localPort", 8080);
        num_of_threads_ = http_proxy_json_object.get<int>        ("numOfThreads", 1);
    }
    catch(std::exception & e)
    {
        Logger::log(e.what(), Logger::LOG_LEVEL::FATAL);
    }
}

Config& Config::get_config(const std::string& file_path)
{
    static Config config(file_path);
    return config;
}
