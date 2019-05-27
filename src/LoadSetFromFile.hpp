#include"TestsConfig.hpp"
#include<set>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
class FileImportSet
{
private:
	std::set<std::string> port;
	std::set<std::string> IP;
	std::set<std::string> blockWordsSet;
	static FileImportSet *s_instance;

public:
	FileImportSet()
	{
		//if (fileName == test_config("IP_BLACKLIST_CSV"))
		//{
			std::ifstream file;
			file.open(test_config("IP_BLACKLIST_CSV"));
			if (!file.is_open())
			{
				std::cout << "Unable to open the file." << std::endl;
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
		//}
		//else
		std::string line;
		std::ifstream file;
		file.open(test_config("PORTS_FILE"));
		if (!file.is_open())
		{
			std::cout << "Unable to open the file." << std::endl;
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
			std::cout << "Unable to open the file." << std::endl;
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
			std::cout << "Unable to open the file." << std::endl;
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
	std::set<std::string>* get_value()
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