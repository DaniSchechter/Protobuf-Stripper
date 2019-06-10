#include "TestsConfig.hpp"
#include "FileChecks.hpp"
#include "LoadSetFromFile.hpp"
#include <unordered_map>
#include <regex>
#include <set>


//Checks how much requests there sent to the destination Ip from different source Ips,
// if its the first time this ip is accessed,
// verify it's a legitimate site Ip by scanning the csv file for this IP address, return true if it's not legitimate and false if it is.
bool request_density(const std::string& src_ip, const std::string& dst_ip, const std::string& raw_data) {
	// store the data in map as [destination ip, set of source ip's that accessed him]
	static std::unordered_map<std::string, std::set<std::string>> map;
	if (map.find(dst_ip) == map.end())
	{
		// may be malicious, this is the first request from the network to this destination ip
		std::regex r(dst_ip);
		if (regex_search(FileImportSet::instance()->get_black_list_ip(),r))
		{
			map[dst_ip].insert(test_config("BLACK_LIST_IP_IDENTIFIER"));
			return true;
		}
		map[dst_ip].insert(src_ip);
	}
	else
	{
		auto set_element = map[dst_ip].begin();
		std::string first_element = *set_element;
		if (first_element == test_config("BLACK_LIST_IP_IDENTIFIER")) return true;
		map[dst_ip].insert(src_ip);
		//on destination Ips that there accessed from less than 3 source ips, check that the size of the raw data doesn't exceed the defined limit and return true if exceeded.
		if (map[dst_ip].size() <= (long unsigned int)std::stoi(test_config("CHECK_SIZE")))
		{
			if (raw_data.size() > (long unsigned int)std::stoi(test_config("MAX_SIZE")))
				return true;
		}
		

	}
	return false;
}
