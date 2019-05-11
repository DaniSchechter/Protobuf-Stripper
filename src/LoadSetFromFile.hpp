#include<set>
#include"TestsConfig.hpp"
class FileImportSet
{
private:
	 std::set<std::string>* set;
public:
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
			while (!file.eof()) {
				getline(file, line);
				std::stringstream words(line);
				set->insert(std::getline(words, word, ',')[2]);
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
			set->insert(line);
		}
		file.close();
	}
	std::set<std::string>* get_set()
	{
		return set;
	}
};