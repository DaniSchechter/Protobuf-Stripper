

#include "server.hpp"

#include <boost/bind.hpp>

server::server(
    const std::string& address, 
    const std::string& port,
    const std::string& configuration_file, 
    std::size_t thread_pool_size)
      : thread_pool_size_(thread_pool_size),
        signals_(io_service_),
        acceptor_(io_service_, boost::asio::ip::tcp::endpoint(address,port)),
        new_connection_ptr_()    
{
    // Register exit signals
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.async_wait(
        server::handle_stop
    );

    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
    boost::asio::ip::tcp::endpoint endpoint = *iterator;
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    start_accept();
}

void 
server::run()
{
  // Create a pool of threads to run all of the io_services.
  for (std::size_t i = 0; i < thread_pool_size_; ++i)
  {
    thread_pool_.create_thread(
      [ &io_service_ = io_service_ ]()
      {
         io_service_.run(); 
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
    new connection(io_service_)
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
server::handle_accept(const boost::system::error_code& error)
{
  if (!error)
  {
    new_connection_ptr_->start();
  }

  start_accept();
}

void 
server::handle_stop()
{
  io_service_.stop();
}