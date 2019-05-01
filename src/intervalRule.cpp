#include "intervalRule.hpp"

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
	static std::unordered_map<requestID, requestInterval, my_string_hash> map; // maybe to open a class with the map

	requestID key(srcIP, dstIP);

	long duration = calculateDuration();

	//if the source and destination ip doesn't exist in the map
	if (map.find(key) == map.end())
	{
		std::cout << "1\n";
		requestInterval interval(duration, 0);
		//inset a new instance with the interval-o to the map
		map[key] = interval;
		
		return false;
		
	}
	else
	{
		//if the interval is zero
		if (map[key].getInterval() == 0)
		{
			std::cout << "2\n";
			requestInterval interval(duration, duration - map[key].getRecieveTime());
			//calculate the new interval and save in in the map
			map[key] = interval;
			return false;
		}
		else
		{
			// checks if the message is sent with the same frequanty
			if (map[key].getInterval() - (duration - map[key].getRecieveTime())<500 && (duration - map[key].getRecieveTime()) - map[key].getInterval()<500)
			{
				std::cout << "3\n";
				requestInterval interval(duration, map[key].getInterval());
				map[key] = interval;
				return true;
			}
			else
			{
				std::cout << "4\n";
				// change the interval to zero
				requestInterval interval(duration, 0);
				map[key] = interval;
				return false;
			}
		}
	}
}
