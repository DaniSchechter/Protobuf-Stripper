#ifndef SECURE_CONTEX_FACTORY_HPP_
#define SECURE_CONTEX_FACTORY_HPP_

#include <boost/asio/ssl/context.hpp> 
#include <mutex>
#include <unordered_set>


struct SecureContextFactory
{
    SecureContextFactory() = delete;

    // Get the host from http message
    static std::unique_ptr<boost::asio::ssl::context> create_context(const std::string& domain);
    
    static std::string get_password() { return "test"; }

    static std::unordered_set<std::string> host_certificate_set_;
    static std::mutex certificate_map_lock_;
};

#endif //SECURE_CONTEX_FACTORY_HPP_