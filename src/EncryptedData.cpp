#include "EncryptedData.hpp"
#include <algorithm>
#include <unordered_map>
#include <math.h>
//calculate the entropy of the raw data to determine if if was encrypted.
double Entropy(const std::string& str)
{
	std::unordered_map<char, int> map;
	int string_lenth = str.length();
	for (char ch : str)
	{
		if (map.find(ch) == map.end())
		{
			map[ch] = 1;
		}
		else {
			map[ch] += 1;
		}
	}
	double result = 0.0;
	std::for_each(map.begin(), map.end(),
	[&](std::pair<char, int > maprow)
	{
	double frequency = (double)maprow.second / string_lenth;
	result -= frequency * (std::log(frequency) / log(2));
	});
	map.erase(map.begin(),map.end());
	return result;
}