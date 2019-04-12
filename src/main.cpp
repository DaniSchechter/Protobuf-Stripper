#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/log/trivial.hpp>

#include "server.hpp"
#include "config.hpp"
#include "logger.hpp"

int main(int argc, char* argv[])
{
  std::unique_ptr<Config> config;

  try
  {

    if(argc > 1)
    {
      config = std::make_unique<Config>(Config::get_config(argv[1]));
    }
    else
    {
      config = std::make_unique<Config>(Config::get_config());
    }

    // Initialize the server.
    // TODO changr to normal cast from std
    server s(std::move(config));

    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    Logger::log(e.what(), Logger::LOG_LEVEL::FATAL);
  }

  return 0;
}