

#include "server.hpp"

server::server(
    const std::string& address, 
    const std::string& port,
    const std::string& thread_pool_size)
      : io_context_(new boost::asio::io_context),
        work_guard_(boost::asio::make_work_guard(*io_context_)),
        localhost_address_(boost::asio::ip::address_v4::from_string(address)),
        localhost_port_(boost::lexical_cast<std::size_t>(port)),
        thread_pool_size_(boost::lexical_cast<std::size_t>(thread_pool_size)) ,
        signals_(*io_context_),
        acceptor_(*io_context_),
        connection_bridge_( )
{
  std::cout << "in server ctor" << std::endl;
    // Register exit signals
    signals_.add(SIGINT);
    signals_.add(SIGTERM); 
    signals_.async_wait(boost::bind(&server::handle_stop, this));
    

    boost::asio::ip::tcp::endpoint endpoint(localhost_address_, localhost_port_);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    start_accept();
}

void server::run()
{
  std::cout << "in run" << std::endl;
  // Create a pool of threads to run all of the io_contexts.
  for (std::size_t i = 0; i < thread_pool_size_; ++i)
  {
    std::cout << thread_pool_size_ << std::endl;
    // Each created thread should wait for completion queue handlers
    thread_pool_.create_thread( boost::bind( &server::WorkerThread, this ));
  }
  // Wait for all threads to finish their work
  thread_pool_.join_all();

  std::cout << "finished all threads" << std::endl;

}

void server::WorkerThread( )
{
  try
	{

		io_context_->run();
	}
	catch( std::exception & ex )
	{
		std::cout << ex.what() << std::endl;
	}
}

void server::start_accept()
{
  // Create a new bridge to handle the next connetion from a client
  connection_bridge_.reset(
    new bridge( io_context_ )
  );
  std::cout << "starting accept" << std::endl;
  
  // Start accepting from client's socket associated with the bridhe
  acceptor_.async_accept(
    connection_bridge_->client_socket(),
    boost::bind(
      &server::handle_accept, 
      this,
      boost::asio::placeholders::error)
    );
}

// // Handle curernt bridge's client connection 
void server::handle_accept(const boost::system::error_code& error)
{

  std::cout << "handeling accept" << std::endl;
  if (!error)
  {
    connection_bridge_->start();
  }

  start_accept();
}


void server::handle_stop()
{
  std::cout << "exiting" << std::endl;
  io_context_->stop();
}
