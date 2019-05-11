#include <unordered_map>
#include <string>
#include <set>
#include"LoadSetFromFile.hpp"
bool port_density(const std::string& srcIP, const std::string& dstIP, const std::string& srcPort, const std::string& dstPort) {
	static std::unordered_map< std::string, std::set<std::string>> port_map;
	if (port_map.find(dstPort) == port_map.end()) //If this port wasn't accessed before
	{
		port_map[dstPort].insert(srcIP);
		return false;
	}
	std::string temp;
	temp = *(std::find(port_map[dstPort].begin(), port_map[dstPort].end(), srcIP));
	if (temp != port_map[dstPort].end())//Port already saved in map
	{
		return false;
	}
	port_map[dstPort].insert(srcIP);
	if (port_map[dstPort].size() >= std::stoi(test_config("PORTS_MIN_COUNT")))
	{ 
		static FileImportSet Ports;
		if (Ports.get_set()->empty())
		{
			Ports= FileImportSet(test_config("PORTS_FILE"));
		}
		return !Ports.get_set()->find(dstPort) //Check if known port or Unknown port, High Usage 
	}
	return false;
}
