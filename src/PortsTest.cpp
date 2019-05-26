#include <unordered_map>
#include <string>
#include <set>
#include"LoadSetFromFile.hpp"
FileImportSet *FileImportSet::s_instance = 0;
bool port_density(const std::string& srcIP, const std::string& dstIP, const std::string& srcPort, const std::string& dstPort)
{
	static std::unordered_map< std::string, std::set<std::string>> port_map;

	if (port_map.find(dstPort) == port_map.end())
		port_map[dstPort].insert(srcIP);
	else
	{
		if (port_map[dstPort].find(srcIP) == port_map[dstPort].end())
			port_map[dstPort].insert(srcIP);
	}
	if (port_map[dstPort].size() >= std::stoi(test_config("PORTS_MIN_COUNT")))
	{ 
		if (FileImportSet::instance()->get_set("port")->empty())
			Ports= FileImportSet(test_config("PORTS_FILE"));

		return Ports.get_set("port").find(dstPort)== Ports.get_set("port").end(); //Check if known port or Unknown port, High Usage 

	}
	return false; 
}
