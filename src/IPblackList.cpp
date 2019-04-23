#include "blacklistIp.h"
#include "httprequest.hpp"
#define API_KEY "Ns1qtHUPHey9vPceRleElhOX6bf18jb6QooeJrncJ7rcvk6v";
#define USER_ID "alonbl";
#define MAX_SIZE 10000000
bool requestdensity(std::string srcIP, std::string dstIP, std::string rawdata) {

		static std::unordered_map<destinationIp, std::list<std::string>> map;

		destinationIp* key = new destinationIp(dstIP);
		if (map.find(*key) == map.end())
		{
			std::list<std::string> srclist;
			srclist.push_front(srcIP);
			map[*key] = srclist;
			// may be malicious, this is the first request from the network to this destination ip, make further checks on the destination ip
			// check if the ip is legitimate site
			try
			{
				std::string tmp = "ip=" + dstIP;
				http::Request request("https://neutrinoapi.com/ip-blocklist"+ tmp);
				// send a get request
				std::vector<std::string> vectortmp;
				std::string userid = USER_ID;
				std::string apikey = API_KEY;
				vectortmp.push_back("user-id:" + userid);
				vectortmp.push_back("api-key:" + apikey);

				http::Response response = request.send("GET","", vectortmp);
				char* res;
				for (int i = 0; i<response.body.size(); i++) {
					res[i] = response.body.data()[i];
				}
				std::string strtemp(res);
				std::size_t found = strtemp.find("true");
				if (found != std::string::npos)
					return false;

			}
			catch (const std::exception& e)
			{
				std::cerr << "Request failed, error: " << e.what() << std::endl;
			}
		}
		else 
		{
			std::string temp;
			std::list<std::string> srclist = map[*key];
			temp = *(std::find(srclist.begin(), srclist.end(), srcIP));
			if (temp != *srclist.end())
			{
				//already exists 
			}
			else
			{
				srclist.push_front(srcIP);
				map[*key] = srclist;
				// if list len is less then 3 check the size of the 
				if (srclist.size() <= 3)
				{
					if (rawdata.size() > MAX_SIZE)
						return false;
				}
			}
		}

	}