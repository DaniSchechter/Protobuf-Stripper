#ifndef SERVER_HPP_
#define SERVER_HPP_

#include <boost/asio/io_context.hpp>
#include <boost/system/error_code.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/thread/thread.hpp>

#include "connection.hpp"
#include "connection_handler.hpp"

#include <string>

class server 
{

public:

    explicit server(
        const std::string& address, 
        const std::string& port,
        const connection_handler& connection_handler,
        const std::string& configuration_file, 
        std::size_t thread_pool_size
    );

    // Initialize thread pool
    void run();

private:

    // Start first accept
    void start_accept();

    // Handle completion of an asynchronous accept operation
    void handle_accept(const boost::system::error_code& e);

    // Handle a request to stop the server
    void handle_stop();

    // The number of completion handlers threads
    std::size_t thread_pool_size_;

    // The number of completion handlers threads
    boost::thread_group thread_pool_;

    // The io_context used to perform asynchronous operations
    boost::asio::io_context io_context_;

    // The signal_set is used to register for process termination notifications
    boost::asio::signal_set signals_;

    // Acceptor used to listen for incoming connections
    boost::asio::ip::tcp::acceptor acceptor_;

    // The next connection to be accepted
    connection_ptr new_connection_ptr_;

    // Connection handler for all new incoming connections
    connection_handler connection_handler_;
};

#endif // SERVER_HPP_