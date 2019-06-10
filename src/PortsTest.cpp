#include <map>
#include <string>
#include <set>
#include <regex>
#include"LoadSetFromFile.hpp"
#include <utility>
#include <time.h>
#define day 86400

bool port_density(const std::string& src_ip,const std::string& dst_ip,const std::string& dstPort)
{
	static std::map< std::pair<std::string,std::string>, std::set<std::string>> port_map;
	static time_t start_time = time(0);
	if(difftime(time(0),start_time) > day)
	{
	port_map.erase(port_map.begin(),port_map.end());
	start_time = time(0);
	}
	std::pair<std::string,std::string> s_pair(dst_ip,dstPort);
	if (port_map.find(s_pair) == port_map.end())
		port_map[s_pair].insert(src_ip);
	else
	{
		if (port_map[s_pair].find(src_ip) == port_map[s_pair].end())
			port_map[s_pair].insert(src_ip);
	}
	if (port_map[s_pair].size() >= (long unsigned int)std::stoi(test_config("PORTS_MIN_COUNT")))
	{
		std::regex r(dstPort);
		return !regex_search(FileImportSet::instance()->get_known_ports(),r); //Check if known port or Unknown port, High Usage 
	}
	return false; 
}
