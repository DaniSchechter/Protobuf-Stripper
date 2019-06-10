#include <unordered_map>
#include <string>
#include <set>
#include <regex>
#include"LoadSetFromFile.hpp"


bool port_density(const std::string& srcIP,const std::string& dstPort)
{
	static std::unordered_map< std::string, std::set<std::string>> port_map;

	if (port_map.find(dstPort) == port_map.end())
		port_map[dstPort].insert(srcIP);
	else
	{
		if (port_map[dstPort].find(srcIP) == port_map[dstPort].end())
			port_map[dstPort].insert(srcIP);
	}
	if (port_map[dstPort].size() >= (long unsigned int)std::stoi(test_config("PORTS_MIN_COUNT")))
	{
		std::regex r(dstPort);
		return !regex_search(FileImportSet::instance()->get_known_ports(),r); //Check if known port or Unknown port, High Usage 
	}
	return false; 
}
