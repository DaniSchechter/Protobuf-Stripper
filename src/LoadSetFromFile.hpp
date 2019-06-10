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
	std::string block_words;
	std::string malicious_ip;
	std::string black_list_ip;
	std::string known_ports;
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
				black_list_ip+=rowvec[2];
				black_list_ip+="@";
			}
			file.close();
		file.open(test_config("PORTS_FILE"));
		if (!file.is_open())
		{
			Logger::log( "OPEN_FILE_ERROR, unable to open file " + test_config("PORTS_FILE"), Logger::LOG_LEVEL::FATAL);
			return;
		}
		while (!file.eof())
		{
			getline(file, line);
			known_ports+=line;
			known_ports+="@";
			
		}
		file.close();

		file.open(test_config("MALICIOUS_IP_FILE"));
		malicious_ip="";
		if (!file.is_open())
		{
			Logger::log( "OPEN_FILE_ERROR, unable to open file " + test_config("MALICIOUS_IP_FILE"), Logger::LOG_LEVEL::FATAL);
			return;
		}
		while (!file.eof())
		{
			getline(file, line);
			malicious_ip+=line;
			malicious_ip+="@";
		}
		file.close();


		file.open(test_config("BLOCK_WORDS_FILE"));
		block_words="";
		if (!file.is_open())
		{
			Logger::log( "OPEN_FILE_ERROR, unable to open file " + test_config("BLOCK_WORDS_FILE"), Logger::LOG_LEVEL::FATAL);
			return;
		}
		while (!file.eof())
		{
			getline(file, line);
			block_words+=line;
		}
		file.close();
	}
	std::string get_block_words()
	{
		return block_words;
	}
	std::string get_malicious_ip()
	{
		return malicious_ip;
	}
	std::string get_black_list_ip()
	{
		return black_list_ip;
	}
	std::string get_known_ports()
	{
		return known_ports;
	}
	static FileImportSet* instance()
	{
		if (!s_instance)
			s_instance = new FileImportSet;
		return s_instance;
	}
};
