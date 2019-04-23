#include "intervalRule.h"

long calculateDuration()
{
	auto now = std::chrono::system_clock::now();
	auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);

	auto value = now_ms.time_since_epoch();
	return value.count();
}


bool isInterval(const std::string& srcIP, const std::string& dstIP)
{
	static std::unordered_map<requestID, requestInterval, my_string_hash> map; // maybe to open a class with the map

	requestID key(srcIP, dstIP);

	long duration = calculateDuration();


	if (map.find(key) == map.end())
	{
		requestInterval interval(duration, 0);
		map[key] = interval;
		return false;
	}
	else
	{
		if (map[key].getInterval() == 0)
		{
			requestInterval interval(duration, duration - map[key].getRecieveTime());
			map[key] = interval;
			return false;
		}
		else
		{
			if (map[key].getInterval() - (duration - map[key].getRecieveTime())<0.5 || (duration - map[key].getRecieveTime()) - map[key].getInterval()<0.5)
			{
				requestInterval interval(duration, map[key].getInterval());
				map[key] = interval;
				return true;
			}
			else
			{
				requestInterval interval(duration, 0);
				map[key] = interval;
				return false;
			}
		}
	}
}