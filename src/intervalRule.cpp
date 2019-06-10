#include "intervalRule.hpp"

#include "hashOverride.hpp"
#include <iostream>
#include <unordered_map> 
#include <ctime>
#include <chrono>
#include <utility> 


long calculateDuration()
{
	auto now = std::chrono::system_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

	auto value = now_ms.time_since_epoch();
	return value.count();
}


bool isInterval(const std::string& srcIP, const std::string& dstIP)
{

	//the map saves the data about the packets (source ip, destination ip, send time, interval)
	static std::unordered_map<std::pair <std::string, std::string>, std::pair <std::time_t, std::time_t>, my_string_hash> map; 
	//requestID key(srcIP, dstIP);
	long duration = calculateDuration();

	//if the source and destination ip doesn't exist in the map
	if (map.find(std::make_pair(srcIP, dstIP)) == map.end())
	{
		//requestInterval interval(duration, 0);
		//inset a new instance with the interval-o to the map
		map[std::make_pair(srcIP, dstIP)] = std::pair <std::time_t, std::time_t>(duration,0);

		return false;
	}
	else
	{
		//if the interval is zero
		if (map[std::make_pair(srcIP, dstIP)].second == 0)
		{
			//requestInterval interval(duration, duration - map[std::make_pair(srcIP, dstIP)].getRecieveTime());
			//calculate the new interval and save in in the map
			if ((duration - map[std::make_pair(srcIP, dstIP)].first)>180000)
			{
   				map[std::make_pair(srcIP, dstIP)] = std::pair <std::time_t, std::time_t>(duration, duration - map[std::make_pair(srcIP, dstIP)].first);;
   				return false;
			}
		}
		else
		{
   			if ((duration - map[std::make_pair(srcIP, dstIP)].first)>180000)
   			{
				// checks if the message is sent with the same frequanty
   				if (map[std::make_pair(srcIP, dstIP)].second - (duration - map[std::make_pair(srcIP, dstIP)].first) < INTERVAL_ERROR && (duration - map[std::make_pair(srcIP, dstIP)].first) - map[std::make_pair(srcIP, dstIP)].second < INTERVAL_ERROR)
   				{
   					//requestInterval interval(duration, map[key].getInterval());
   					map[std::make_pair(srcIP, dstIP)] = std::pair <std::time_t, std::time_t>(duration, map[std::make_pair(srcIP, dstIP)].second);
   					return true;
				}
				else
   				{
   					// change the interval to zero
   					//requestInterval interval(duration, 0);
   					map[std::make_pair(srcIP, dstIP)] = std::pair <std::time_t, std::time_t>(duration, 0);
   					return false;
   				}
			}

		}
	}
	return false;
}

