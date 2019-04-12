#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include "server.hpp"

int main(int argc, char* argv[])
{
  try
  {
    // // Check command line arguments.
    // if (argc != 4)
    // {
    //   std::cerr << "Usage: http_server <address> <port> <threads>\n";
    //   std::cerr << "  For IPv4, try:\n";
    //   std::cerr << "    receiver 0.0.0.0 80 1 .\n";
    //   std::cerr << "  For IPv6, try:\n";
    //   std::cerr << "    receiver 0::0 80 1 .\n";
    //   std::cout << std::endl;
    //   return 1;
    // }

    // Initialise the server.
    // TODO changr to normal cast from std
    server s("127.0.0.1", "80", "3");

    // Run the server until stopped.
    s.run();
  }
  catch (std::exception& e)
  {
    std::cerr << "exception: " << e.what() << "\n";
  }

  return 0;
}