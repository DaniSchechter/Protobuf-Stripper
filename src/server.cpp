

#include "server.hpp"
#include "logger.hpp"

server::server(std::unique_ptr<Config> config)
      : io_context_(new boost::asio::io_context),
        work_guard_(boost::asio::make_work_guard(*io_context_)),
        localhost_address_(boost::asio::ip::address_v4::from_string(config->local_host_)),
        localhost_port_(boost::lexical_cast<std::size_t>(config->local_port_)),
        thread_pool_size_(boost::lexical_cast<std::size_t>(config->num_of_threads_)) ,
        signals_(*io_context_),
        acceptor_(*io_context_),
        connection_bridge_( )
{
    Logger::log(
      "Starting server: " + 
      config->local_host_ + ":" + boost::lexical_cast<std::string>(config->local_port_) + 
      " with " + boost::lexical_cast<std::string>(config->num_of_threads_) + " Threads"
    ,Logger::LOG_LEVEL::WARNING); 
    

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
  // TODO log
  boost::thread_group worker_threads;

  // Create a pool of threads to run all of the io_contexts.
  for (std::size_t i = 0; i < thread_pool_size_; ++i)
  {
    // Each created thread should wait for completion queue handlers
    thread_pool_.create_thread( boost::bind( &server::WorkerThread, this ));
  }
  // Wait for all threads to finish their work
  thread_pool_.join_all();

  Logger::log("Joined all threads" , Logger::LOG_LEVEL::WARNING);

}

void server::WorkerThread( )
{
  try
	{

		io_context_->run();
	}
	catch( std::exception & ex )
	{
		Logger::log(ex.what() , Logger::LOG_LEVEL::FATAL);
	}
}

void server::start_accept()
{
  // Create a new bridge to handle the next connetion from a client
  connection_bridge_.reset(
    new bridge( io_context_ )
  );
  
  // Start accepting from client's socket associated with the bridhe
  acceptor_.async_accept(
    connection_bridge_->client_socket(),
    boost::bind(
      &server::handle_accept, 
      this,
      boost::asio::placeholders::error)
    );
}

// Handle curernt bridge's client connection 
void server::handle_accept(const boost::system::error_code& error)
{
  Logger::log("New client connected - handling accept" , Logger::LOG_LEVEL::WARNING);
  if (!error)
  {
    connection_bridge_->start();
  }

  start_accept();
}


void server::handle_stop()
{
  Logger::log("Server exits" , Logger::LOG_LEVEL::WARNING);
  io_context_->stop();
  io_context_.reset();
}
