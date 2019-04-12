#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <boost/asio/io_context.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/thread/thread.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>

#include "bridge.hpp"

#include <iostream>
#include <string>

class server 
{

public:
    // TODO add const for const methods that dont change class vars
    explicit server(
        const std::string& address, 
        const std::string& port,
        const std::string& thread_pool_size
    );

    // Initialize thread pool and start first accept
    void run();

private:

    // Start first accept
    void start_accept();

    // Handle completion of an asynchronous accept operation
    void handle_accept(const boost::system::error_code& e);

    // Handle a request to stop the server
    void handle_stop();

    void WorkerThread( boost::shared_ptr< boost::asio::io_context > io_context );

    // The io_context used to perform asynchronous operations
    boost::shared_ptr<boost::asio::io_context> io_context_;

    //local host ipv4 address
    boost::asio::ip::address_v4 localhost_address_;

    //local host ipv4 port
    unsigned short localhost_port_;

    // The number of completion handlers threads
    std::size_t thread_pool_size_;

    // Completion handlers threads
    boost::thread_group thread_pool_;

    // The signal_set is used to register for process termination notifications
    boost::asio::signal_set signals_;

    // Acceptor used to listen for incoming connections
    boost::asio::ip::tcp::acceptor acceptor_;

    // The next connection to be accepted
    boost::shared_ptr<bridge> connection_bridge_;

};

#endif // SERVER_HPP_