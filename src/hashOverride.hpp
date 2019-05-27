#ifndef HASH_OVERRIDE_HPP_

#define HASH_OVERRIDE_HPP_


#include <ctime>

//Hash class for the unordered map with the key field requestID
class my_string_hash
{
public:
	
	//overide of the hash function for the requestID class
	size_t calculateHash(const std::pair<std::string, std::string>& key) const
	{
		size_t hashVal = 0;
		for (char ch : key.first)
			hashVal = 37 * hashVal + ch;
		for (char ch : key.second)
			hashVal = 37 * hashVal + ch;
		return hashVal;
	}

	//overide of the equal operator
	size_t operator()(const std::pair <std::string, std::string>& key) const
	{
		return calculateHash(key);
	}

};

#endif