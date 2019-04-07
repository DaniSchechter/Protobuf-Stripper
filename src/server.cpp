

#include "server.hpp"

#include <boost/bind.hpp>

server::server(
    const std::string& address, 
    const std::string& port,
    const connection_handler& connection_handler,
    const std::string& configuration_file, 
    std::size_t thread_pool_size)
      : thread_pool_size_(thread_pool_size),
        signals_(io_context_),
        acceptor_(io_context_),
        new_connection_(),
        connection_handler_(configuration_file)
    
{
    // Register exit signals
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.async_wait(
        server::handle_stop
    );

    boost::asio::ip::tcp::resolver resolver(io_context_);
    boost::asio::ip::tcp::endpoint endpoint =
    *resolver.resolve(address, port).begin();
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    start_accept();
}

void 
server::run()
{
  // Create a pool of threads to run all of the io_contexts.
  for (std::size_t i = 0; i < thread_pool_size_; ++i)
  {
    thread_pool_.create_thread(
      [ &io_context_ = io_context_ ]()
      {
         io_context_.run(); 
      }
    );
  }

  // Wait for all threads to finish their work
  thread_pool_.join_all();
}

void 
server::start_accept()
{

  new_connection_ptr_.reset(
    new connection(io_context_, connection_handler_)
  );

  acceptor_.async_accept(
    new_connection_ptr_->socket(),
    boost::bind(
      &server::handle_accept, 
      this,
      boost::asio::placeholders::error)
    );
}

void 
server::handle_accept(const boost::system::error_code& e)
{
  if (!e)
  {
    new_connection_ptr_->start();
  }

  start_accept();
}

void 
server::handle_stop()
{
  io_context_.stop();
}