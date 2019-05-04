#include "PortsTest.hpp"
#include"TestsConfig.hpp"
#include "FileChecks.hpp"
bool request_density(const std::string& srcIP, const std::string& dstIP, const std::string& srcPort, const std::string& dstPort) {
	static std::unordered_map< std::string, std::set<std::string>> map;
	if (map.find(dstPort) == map.end())
	{
		map[dstPort].insert(srcIP);
		return false;
	}
	else
	{
		std::string temp;
		temp = *(std::find(map[dstPort].begin(), map[dstPort].end(), srcIP));
		if (temp != *map[dstPort].end())
		{
			return false;
		}
		else
		{
			map[dstPort].insert(srcIP);
			if (map[dstPort].size() >= atoi(test_config("PORTS_MIN_COUNT").c_str()))
			{
				if (find_str_in_file(test_config("PORTS_FILE"), dstPort))
				{
					return false;
				}
				else { //Unknown port, High Usage 
					return true;
				}
			}
		}
		return false;
	}

}
