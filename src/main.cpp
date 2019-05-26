#include "server.hpp"
#include "config.hpp"
#include "logger.hpp"

#include <string>
#include <boost/bind.hpp>

int main(int argc, char* argv[])
{
#ifdef DEBUG_MODE
	std::cout << "Running in Debug mode" << std::endl;
#endif

	std::unique_ptr<Config> config;

	try
	{
		std::vector <std::string> compilation_flags;
		if (argc > 1)
		{
			config = std::make_unique<Config>(Config::get_config(argv[1]));
		}
		else
		{
			config = std::make_unique<Config>(Config::get_config());
		}

		// Initialize the server.
		// TODO changr to normal cast from std
		Server server(std::move(config));

		// Run the server until stopped.
		server.run();
	}
	catch (std::exception & e)
	{
		Logger::log(e.what(), Logger::LOG_LEVEL::FATAL);
	}
	return 0;
}