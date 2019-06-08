#define SSL_KEYS_DIRECOTRY "./.sslkeys"
#define LEAF_CERTIFICATE_SCRIPT "./scripts/generate_certificate.sh " SSL_KEYS_DIRECOTRY " "

#include "secureContextFactory.hpp"
#include "logger.hpp"
#include "utils.hpp"
#include <boost/bind.hpp> 
#include <boost/lexical_cast.hpp>

std::unordered_set< std::string > SecureContextFactory::host_certificate_set_;
std::mutex SecureContextFactory::certificate_map_lock_;

std::unique_ptr<boost::asio::ssl::context> SecureContextFactory::create_context(const std::string& domain)
{
    std::string common_name;
    if( Utils::fetch_common_name(domain, common_name) == Utils::COMMON_NAME_ERROR )
    {
        Logger::log(
            "No match while parsing common name out of the domain: " + domain, Logger::LOG_LEVEL::FATAL
        );
        return nullptr;
    }

    // If this is a new domain
    SecureContextFactory::certificate_map_lock_.lock();
    if (host_certificate_set_.find(common_name) == host_certificate_set_.end())
    {
        Logger::log(
            "Generating a new certificate for\nFull domain: " + domain +
            "\nCommon name: " + common_name, Logger::LOG_LEVEL::INFO
        );
        // Set the new certificate file for the requested domain
        std::string script_command = boost::lexical_cast<std::string>(LEAF_CERTIFICATE_SCRIPT) + common_name;
        if(system(script_command.c_str()) != 0)
        {
            Logger::log(
            "Error generating a new certificate for" + common_name, Logger::LOG_LEVEL::FATAL
            );
            return nullptr;
        }
        SecureContextFactory::host_certificate_set_.insert(common_name);
    }      
    SecureContextFactory::certificate_map_lock_.unlock();
    // Initialize the context

    boost::asio::ssl::context ctx = boost::asio::ssl::context(boost::asio::ssl::context::sslv23);
    ctx.set_options(
        boost::asio::ssl::context::default_workarounds
        | boost::asio::ssl::context::no_sslv2
        | boost::asio::ssl::context::no_sslv3
    );
    ctx.set_password_callback(boost::bind(&SecureContextFactory::get_password));
    ctx.use_certificate_chain_file(SSL_KEYS_DIRECOTRY + ("/" + common_name) + "/" + common_name + ".crt");
    ctx.use_private_key_file(SSL_KEYS_DIRECOTRY  + ("/" + common_name) + "/" + common_name + ".key", boost::asio::ssl::context::pem);

    return std::make_unique<boost::asio::ssl::context>(std::move(ctx));
}
