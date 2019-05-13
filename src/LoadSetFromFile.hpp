#include"TestsConfig.hpp"
#include<set>
#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
class FileImportSet
{
private:
	 std::set<std::string> set;
	 static FileImportSet *s_instance;
public:
	FileImportSet(){}
	FileImportSet(const std::string& fileName)
	{
		if (fileName == test_config("IP_BLACKLIST_CSV"))
		{
			std::ifstream file;
			file.open(fileName);
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
				set.insert(rowvec[2]);
			}
			file.close();
			return;
		}
		//else
		std::string line;
		std::ifstream file;
		file.open(fileName);
		if (!file.is_open())
		{
			std::cout << "Unable to open the file." << std::endl;
			return;
		}
		while (!file.eof())
		{
			getline(file, line);
			set.insert(line);
		}
		file.close();
	}
	std::set<std::string> get_set()
	{
		return set;
	}
	std::set<std::string>* get_value()
	{
		return &set;
	}
	static FileImportSet* instance()
	{
		if (!s_instance)
			s_instance = new FileImportSet;
		return s_instance;
	}
};