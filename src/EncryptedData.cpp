#include "EncryptedData.hpp"
//calculate the entropy of the raw data to determine if if was encrypted.
double Entropy(const std::string& str)
{
	static std::unordered_map<char, int> map;
	int string_lenth = str.length();
	for (char ch in str)
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
	result -= frequency * (log(frequency) / log(2));
	});
	return result;
}