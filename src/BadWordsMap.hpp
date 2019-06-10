#ifndef BAD_WORDS_MAP_HPP

#define BAD_WORDS_MAP_HPP

#include <string>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include "filesHandler.hpp"
#include <iostream>
#include "TestsConfig.hpp"


class BadWordsMap
{
	std::unordered_map<std::string, int> blockWordsMap;
	static BadWordsMap *s_instance;
	BadWordsMap()
	{		
		if (blockWordsMap.empty() == false)
			return;		

		std::fstream file;

		file.open(test_config("MALICIOUS_WORDS_FILE"), std::ios::in);
		std::vector<std::string> splitLine;
		//checks if the file opened properlly
		if (!file.is_open())
			return;

		std::string line;

		std::string intermediate;

		while (!file.eof())
		{
			//get a line from the file
			getline(file, line);

			if (line.compare("") != 0)
			{
				std::stringstream split(line);

				//split the line from the value (ex.   virus-3)
				while (getline(split, intermediate, '-'))
				{
					splitLine.push_back(intermediate);
				}

				blockWordsMap.insert(std::make_pair(splitLine[0], stoi(splitLine[1])));

				splitLine.clear();
			}
		}
		file.close();
	}
public:
	std::unordered_map<std::string, int>* get_value()
	{
		return &blockWordsMap;
	}
	static BadWordsMap *instance()
	{
		if (!s_instance)
			s_instance = new BadWordsMap();
		return s_instance;
	}
};

#endif