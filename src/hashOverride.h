

#include "requestID.h"


#include <iostream> 
#include <unordered_map> 
#include <stdio.h>
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>




class my_string_hash
{
public:
	size_t calculateHash(const requestID & key) const
	{
		size_t hashVal = 0;
		for (char ch : key.getSrcIp())
			hashVal = 37 * hashVal + ch;
		for (char ch : key.getDstIp())
			hashVal = 37 * hashVal + ch;
		return hashVal;
	}

	size_t operator()(const requestID & key) const // <-- don't forget const
	{
		return calculateHash(key);
	}

};