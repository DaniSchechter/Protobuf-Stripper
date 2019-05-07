#ifndef hashOverride.hpp

#include "requestID.hpp"
#include <ctime>

//Hash class for the unordered map with the key field requestID
class my_string_hash
{
public:
	
	//overide of the hash function for the requestID class
	size_t calculateHash(const requestID & key) const
	{
		size_t hashVal = 0;
		for (char ch : key.getSrcIp())
			hashVal = 37 * hashVal + ch;
		for (char ch : key.getDstIp())
			hashVal = 37 * hashVal + ch;
		return hashVal;
	}

	//overide of the equal operator
	size_t operator()(const requestID & key) const
	{
		return calculateHash(key);
	}

};

#endif