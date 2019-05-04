#include "HashOverrideDestIp.hpp"
#include"TestsConfig.hpp"
#include "FileChecks.hpp"
//Checks how much requests there sent to the destination Ip from different source Ips, if its the first time this ip is accessed, verify it's a legitimate site Ip by scanning the csv file for this IP address, return true if it's not legitimate and false if it is.
bool request_density(const std::string& srcIP, const std::string& dstIP, const std::string& rawdata) {
	// store the data in map as [destination ip, set of source ip's that accessed him]
	static std::unordered_map<destinationIp, std::set<std::string>, my_string_hash> map;
	destinationIp* key = new destinationIp(dstIP);
	if (map.find(*key) == map.end())
	{
		// may be malicious, this is the first request from the network to this destination ip
		if (read_csv_ip_blacklist(test_config("IP_BLACKLIST_CSV"), dstIP)) {
			map[*key].insert("BLACK_LIST");
			return true;
		}
		map[*key].insert(srcIP);
	}
	else
	{
		auto set_element = map[*key].begin();
		std::string first_element = *set_element;
		if (strcmp(first_element.c_str(), "BLACK_LIST") == 0) return true;
		std::string temp;
		temp = *(std::find(map[*key].begin(), map[*key].end(), srcIP));
		if (temp != *map[*key].end())
		{
			return false;
		}
		else
		{
			map[*key].insert(srcIP);
			//on destination Ips that there accessed from less than 3 source ips, check that the size of the raw data doesn't exceed the defined limit and return true if exceeded.
			if (map[*key].size() <= atoi(test_config("CHECK_SIZE").c_str()))
			{
				if (rawdata.size() > atoi(test_config("MAX_SIZE").c_str()))
					return true;
			}
		}
		return false;
	}

}
