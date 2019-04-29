#include "HashOverrideDestIp.hpp"
#include"TestsConfig.hpp"
#include"SFML/Network.hpp"
//Checks how much requests there sent to the destination Ip from different source Ips, if its the first time this ip is accessed, send the Ip address to Ip blacklist api to verify it's a legitimate site Ip, return true if it's not legitimate and false if it is.
bool request_density(const std::string& srcIP, const std::string& dstIP, const std::string& rawdata) {
	// store the data in map as [destination ip, set of source ip's that accessed him]
	static std::unordered_map<destinationIp, std::set<std::string>, my_string_hash> map;
	destinationIp* key = new destinationIp(dstIP);
	if (map.find(*key) == map.end())
	{
		std::set<std::string> srcset;
		srcset.insert(srcIP);
		map[*key] = srcset;
		// may be malicious, this is the first request from the network to this destination ip
		// check if the ip is a legitimate site
		try
		{
			sf::Http http("https://neutrinoapi.com/ip-blocklist");
			std::string tmp = "?ip=" + dstIP;
			sf::Http::Request request;
			request.setMethod(sf::Http::Request::Get);
			request.setUri(tmp);
			request.setHttpVersion(1, 1); // HTTP 1.1
			std::string userid = test_config("USER_ID");
			std::string apikey = test_config("API_KEY");
			request.setField("user-id:", userid);
			request.setField("api-key:", apikey);
			sf::Http::Response response = http.sendRequest(request);
			std::size_t found = response.getBody().find("true");
			if (found != std::string::npos)
				return true; //ip found in blacklist
			return false; // ip not in blacklists
		}
		catch (const std::exception & e)
		{
			std::cerr << "Request failed, error: " << e.what() << std::endl;
		}
	}
	else
	{
		std::string temp;
		std::set<std::string> srcset = map[*key];
		temp = *(std::find(srcset.begin(), srcset.end(), srcIP));
		if (temp != *srcset.end())
		{
			return false;
		}
		else
		{
			srcset.insert(srcIP);
			map[*key] = srcset;
			//on destination Ips that there accessed from less than 3 source ips, check that the size of the raw data doesn't exceed the defined limit and return true if exceeded.
			if (srcset.size() <= atoi(test_config("CHECK_SIZE").c_str()))
			{
				if (rawdata.size() > atoi(test_config("MAX_SIZE").c_str()))
					return true;
			}
		}
		return false;
	}

}
