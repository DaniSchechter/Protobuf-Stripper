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
			if (line[0] == '#' || line.empty())
				continue;
			auto Pos = line.find("=");
			std::string name = line.substr(0, Pos);
			std::string value = line.substr(Pos + 1);
			if (cname.compare(name) == 0)
				return value;
		}

	}
	else {
		std::cerr << "Error opening config file";
	}
}
