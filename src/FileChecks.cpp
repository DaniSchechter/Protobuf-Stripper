#include "FileChecks.hpp"
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include "logger.hpp"
bool find_str_in_file(const std::string& file_name, const std::string& word)
{
	std::string line;
	std::ifstream file;
	file.open(file_name);
	if (!file.is_open())
	{
		Logger::log( "OPEN_FILE_ERROR, unable to open file " + file_name, Logger::LOG_LEVEL::FATAL);
		return true;
	}
	while (!file.eof())
	{
		getline(file, line);
		if ((line.find(word, 0)) != std::string::npos)
		{
			file.close();
			return true;
		}
	}
	file.close();
	return false;
}
bool read_csv_ip_blacklist(const std::string& file_name, const std::string& IP)
{
	std::ifstream file;
	file.open(file_name);
	if (!file.is_open())
	{
		std::cout << "Unable to open the file." << std::endl;
		return true;
	}
	std::vector<std::string> rowvec;
	std::string line, word, temp;

	while (file >> temp) {
		rowvec.clear();
		getline(file, line);
		std::stringstream words(line);
		while (std::getline(words, word, ',')) {
			rowvec.push_back(word);
		}
		if (rowvec[2].find(IP) != std::string::npos) {
			file.close();
			return true;
		}
	}
	file.close();
	return false;
}