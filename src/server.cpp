#include "server.hpp"
#include "logger.hpp"

#include <string>
#include <boost/lexical_cast.hpp>

Server::Server(std::unique_ptr<Config> config)
      : io_context_(new boost::asio::io_context),
        work_guard_(boost::asio::make_work_guard(*io_context_)),
        thread_pool_size_(boost::lexical_cast<std::size_t>(config->num_of_threads_)) ,
        signals_(*io_context_),
        acceptor_(*io_context_)
{
    Logger::log(
      "Starting server: " + config->local_host_ + ":" + boost::lexical_cast<std::string>(config->local_port_) + 
      " with " + boost::lexical_cast<std::string>(config->num_of_threads_) + " Threads"
      ,Logger::LOG_LEVEL::WARNING
    ); 

    // Register exit signals
    signals_.add(SIGINT);
    signals_.add(SIGTERM); 
    signals_.async_wait(boost::bind(&Server::handle_stop, this));
    
    boost::asio::ip::address_v4 localhost_address_ = boost::asio::ip::address_v4::from_string(config->local_host_);

    // Bind and start listening
    boost::asio::ip::tcp::endpoint endpoint(localhost_address_, config->local_port_);
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    std::shared_ptr<BridgeConnector> next_bridge_connector = std::make_shared<BridgeConnector>(io_context_);
  
    // Start accepting from client's socket associated with the bridhe
    try{
      acceptor_.async_accept(
            next_bridge_connector->client_socket(),
            [=](auto error)
            {
              handle_accept(error, next_bridge_connector);
            }
          );
    }
    catch( std::exception & ex )
	  {
      Logger::log(ex.what() , Logger::LOG_LEVEL::FATAL);
	  }
    
}

void Server::run()
{
  boost::thread_group worker_threads;

  // Create a pool of threads to run all of the io_contexts.
  for (std::size_t i = 0; i < thread_pool_size_; ++i)
  {
    // Each created thread should wait for completion queue handlers
    thread_pool_.create_thread( boost::bind( &Server::WorkerThread, this ));
  }
  // Wait for all threads to finish their work
  thread_pool_.join_all();

  Logger::log("Joined all threads" , Logger::LOG_LEVEL::WARNING);

}

void Server::WorkerThread( )
{
  while( true )
  {
		try
		{
			boost::system::error_code ec;
			io_context_->run( ec );
			if( ec )
			{
				Logger::log("Error !, code: " + boost::lexical_cast<std::string>(ec), Logger::LOG_LEVEL::FATAL);
			}
      break;
		}
		catch( std::exception & ex )
		{
			Logger::log("Exception: " + boost::lexical_cast<std::string>(ex.what()), Logger::LOG_LEVEL::FATAL);
		}
  } 
}


void Server::handle_accept(const boost::system::error_code& error, std::shared_ptr<BridgeConnector> bridge_connector)
{
  if(error) return;
  else
  {
    // Start serving the client
    bridge_connector->start();

    // Prepare for next client's connection
    std::shared_ptr<BridgeConnector> next_bridge_connector = std::make_shared<BridgeConnector>(io_context_);
  
    acceptor_.async_accept(
      next_bridge_connector->client_socket(),
      [=](auto error)
      {
        handle_accept(error, next_bridge_connector);
      }
    );
  }
}

void Server::handle_stop()
{
  Logger::log("Server exits..." , Logger::LOG_LEVEL::WARNING);
  io_context_->stop();
  io_context_.reset();
}
