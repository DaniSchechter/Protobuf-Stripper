#include "EncryptedData.hpp"
double Entropy(std::string str)
{
	static std::unordered_map<char, int> map;
	int string_lenth = str.length();
	for (int i = 0; i < string_lenth; i++)
	{
		if (map.find(str[i]) == map.end())
		{
			map[str[i]] = 1;
		}
		else {
			map[str[i]] += 1;
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