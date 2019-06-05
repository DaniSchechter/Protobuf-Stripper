#include"TestsConfig.hpp"
#include <iostream>
#include <fstream>
#include <algorithm>
#include "string.h"

std::string test_config(const std::string& cname)
{
	std::ifstream cFile("include/config.txt");
	if (cFile.is_open())
	{

		std::string line;
		while (getline(cFile, line)) {
			// line.erase(std::remove_if(line.begin(), line.end(), isspace),
			// 	line.end());
			if (line[0] == '#' || line.empty())
				continue;
			auto Pos = line.find("=");
			auto name = line.substr(0, Pos);
			auto value = line.substr(Pos + 1);
			if (strcmp(cname.c_str(), name.c_str()) == 0)
				return value;
		}

	}
	else {
		std::cerr << "Error opening config file";
	}
}
