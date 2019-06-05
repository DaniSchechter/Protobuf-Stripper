#include "TestsConfig.hpp"
#include "FileChecks.hpp"
#include "LoadSetFromFile.hpp"
#include <unordered_map>
#include <set>


//Checks how much requests there sent to the destination Ip from different source Ips,
// if its the first time this ip is accessed,
// verify it's a legitimate site Ip by scanning the csv file for this IP address, return true if it's not legitimate and false if it is.
bool request_density(const std::string& srcIP, const std::string& dstIP, const std::string& rawdata) {
	// store the data in map as [destination ip, set of source ip's that accessed him]
	static std::unordered_map<std::string, std::set<std::string>> map;
	if (map.find(dstIP) == map.end())
	{
		// may be malicious, this is the first request from the network to this destination ip
		
		if (FileImportSet::instance()->get_value("IP")->find(dstIP) != FileImportSet::instance()->get_value("IP")->end())
		{
			map[dstIP].insert(test_config("BLACK_LIST_IP_IDENTIFIER"));
			return true;
		}
		map[dstIP].insert(srcIP);
	}
	else
	{
		auto set_element = map[dstIP].begin();
		std::string first_element = *set_element;
		if (first_element == test_config("BLACK_LIST_IP_IDENTIFIER")) return true;
		map[dstIP].insert(srcIP);
		//on destination Ips that there accessed from less than 3 source ips, check that the size of the raw data doesn't exceed the defined limit and return true if exceeded.
		if (map[dstIP].size() <= std::stoi(test_config("CHECK_SIZE")))
		{
			if (rawdata.size() > std::stoi(test_config("MAX_SIZE")))
				return true;
		}
		

	}
	return false;
}
