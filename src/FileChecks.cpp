#include "FileChecks.hpp"
#include <vector>
#include <sstream>
bool find_str_in_file(const std::string& fileName, const std::string& word)
{
	std::string line;
	std::ifstream file;
	file.open(fileName);
	if (!file.is_open())
	{
		std::cout << "Unable to open the file." << std::endl;
	}
	while (!file.eof())
	{
		getline(file, line);
		if ((line.find(word, 0)) != std::string::npos)
		{
			return true;
		}
	}
	file.close();
	return false;
}
bool read_csv_ip_blacklist(const std::string& fileName, const std::string& IP)
{
	std::ifstream file;
	file.open(fileName);
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
			return true;
		}
	}
	return false;
}