#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <string>
#include <boost/property_tree/json_parser.hpp>
#include <iostream>

class Config
{

public:

    static Config& get_config(const std::string& file_path = "include/config.json");
    void operator=(const Config& other) = delete;

private:

    Config(const std::string& file_path);
   
public:

    std::string local_host_;
    unsigned short local_port_;
    unsigned short num_of_threads_;

};

#endif //CONFIG_HPP_