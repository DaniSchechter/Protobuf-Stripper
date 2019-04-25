#ifndef SERVER_HPP_
#define SERVER_HPP_

#include "config.hpp"
#include "bridge.hpp"
#include "bridgeConnector.hpp"

#include <boost/asio/io_context.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>
#include <boost/thread/thread.hpp>

class Server 
{

public:
    // TODO add const for const methods that dont change class vars
    explicit Server(std::unique_ptr<Config> config);

    // Initialize thread pool and start first accept
    void run();
    
private:

    // Handle new connection frmo a client
    void handle_accept(const boost::system::error_code& error, std::shared_ptr<BridgeConnector> bridge_connector);

    // Handle a request to stop the server
    void handle_stop();

    void WorkerThread( );

    // The io_context used to perform asynchronous operations
    std::shared_ptr<boost::asio::io_context> io_context_;

    // To prevent workers from running out of work
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work_guard_;

    // The number of completion handlers threads
    std::size_t thread_pool_size_;

    // Completion handlers threads
    boost::thread_group thread_pool_;

    // The signal_set is used to register for process termination notifications
    boost::asio::signal_set signals_;

    // Acceptor used to listen for incoming connections
    boost::asio::ip::tcp::acceptor acceptor_;

};

#endif // SERVER_HPP_
