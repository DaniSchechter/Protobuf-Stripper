#include"TestsConfig.hpp"
#include "BadWordsMap.hpp"
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "logger.hpp"

class FileImportSet
{
	std::set<std::string> port;
	std::set<std::string> IP;
	std::set<std::string> blockWordsSet;
	static FileImportSet *s_instance;
public:
	FileImportSet()
	{
			std::ifstream file;
			file.open(test_config("IP_BLACKLIST_CSV"));
			if (!file.is_open())
			{
				Logger::log( "OPEN_FILE_ERROR, unable to open file " + test_config("IP_BLACKLIST_CSV"), Logger::LOG_LEVEL::FATAL);
				return;
			}
			std::string line, word;
			std::vector<std::string> rowvec;
			while (!file.eof()) {
				rowvec.clear();
				getline(file, line);
				std::stringstream words(line);
				while (std::getline(words, word, ',')) {
					rowvec.push_back(word);
				}
				IP.insert(rowvec[2]);
			}
			file.close();
			return;
		file.open(test_config("PORTS_FILE"));
		if (!file.is_open())
		{
			Logger::log( "OPEN_FILE_ERROR, unable to open file " + test_config("PORTS_FILE"), Logger::LOG_LEVEL::FATAL);
			return;
		}
		while (!file.eof())
		{
			getline(file, line);
			port.insert(line);
			
		}
		file.close();

		file.open(test_config("MALICIOUS_IP_FILE"));
		if (!file.is_open())
		{
			Logger::log( "OPEN_FILE_ERROR, unable to open file " + test_config("MALICIOUS_IP_FILE"), Logger::LOG_LEVEL::FATAL);
			return;
		}
		while (!file.eof())
		{
			getline(file, line);
			IP.insert(line);
		
		}
		file.close();


		file.open(test_config("BLOCK_WORDS_FILE"));
		if (!file.is_open())
		{
			Logger::log( "OPEN_FILE_ERROR, unable to open file " + test_config("BLOCK_WORDS_FILE"), Logger::LOG_LEVEL::FATAL);
			return;
		}
		while (!file.eof())
		{
			getline(file, line);
			blockWordsSet.insert(line);

		}
		file.close();
	}
	std::set<std::string> get_set(const std::string& Name)
	{
		if (Name == "port") return port;
		if (Name == "IP" ) return IP;
		if (Name == "word") return blockWordsSet;
	}
	std::set<std::string>* get_value(const std::string& Name)
	{
		if (Name == "port") return &port;
		if (Name == "IP") return &IP;
		if (Name == "word") return &blockWordsSet;
		return NULL;
	}
	static FileImportSet* instance()
	{
		if (!s_instance)
			s_instance = new FileImportSet;
		return s_instance;
	}
};
