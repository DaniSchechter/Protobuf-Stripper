#include "filesHandler.hpp"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>
#include <set>
#include"LoadSetFromFile.hpp"
#include "BadWordsMap.hpp"


FileImportSet *FileImportSet::s_instance = 0;
BadWordsMap *BadWordsMap::s_instance = 0;

//To open the files before the functions and to pass the text in a string variable.
//To call data from disk as little as possible.(efficiency)

int isStrExistsInFile(const std::string& fileName, const std::string& word)
{
	

	if (fileName.compare(test_config("MALICIOUS_WORDS_FILE")) == 0)
	{
		//std::unordered_map<std::string, int> map;
		//map.insert(BadWordsMap::instance()->get_value().begin(), BadWordsMap::instance()->get_value().end());
		
		if (BadWordsMap::instance()->get_value()->find(word) == BadWordsMap::instance()->get_value()->end())
			return 0;
		return (*BadWordsMap::instance()->get_value())[word];

	}
	else
	{
		/*FileImportSet* set = new FileImportSet(fileName);*/
		if (fileName == test_config("BLOCK_WORDS_FILE"))
		{
			if (FileImportSet::instance()->get_value("word")->find(word) == FileImportSet::instance()->get_value("word")->end())
				return 0;
			return BLOCK_WORD;
		}
		if (FileImportSet::instance()->get_value("IP")->find(word) == FileImportSet::instance()->get_value("IP")->end())
			return 0;
		return BLOCK_WORD;
		
		

	}
	

}



void printFile(const std::string& fileName)
{
	std::fstream file;

	file.open(fileName, std::ios::in);
	if (!file.is_open())
		return;
	std::string line;

	while (!file.eof())
	{
		//get a line from the file
		getline(file, line);
			
		//print the line to the screen
		std::cout << line << '\n';
	}
	file.close();
}

void copyFilesContent(const std::string& srcFile, const std::string& dstFile)
{
	remove(dstFile.c_str());

	std::string line;
	std::ofstream file;
	file.open(dstFile, std::ios::out | std::ios::app);

	std::fstream temp;

	temp.open(srcFile, std::ios::in);
	
	//if the file opened properlly
	if (!temp.is_open())
		return;
	//while there is a text to read in the file
	while (!temp.eof())
	{
					
		getline(temp, line);
		if(line.compare("\r\n") != 0 && line.compare("") != 0)
			file << line << "\n";
	}
	file.close();
	temp.close();
	remove(srcFile.c_str());
}



bool deleteLinesFromFile(const std::string& fileName, std::set<std::string> s)
{

	std::fstream file;
	bool flag = false;

	file.open(fileName, std::ios::in);
	
	//if the file opened properlly
	if (!file.is_open())
		return false;
	
	std::string line;

	std::ofstream tempFile;
	tempFile.open("tempFile.csv");
		
	// if there isn't line to remove
	if (s.size() == 0)
		return false;
		
	while (!file.eof())
	{
		//get a line from the file
		getline(file, line);
			
		//if the line from the file isn't in the set
		if (s.find(line) == s.end() && line.compare("\r\n") != 0)
			tempFile << line + "\n";
		else if (line != "")
			flag = true;

	}
	file.close();
	tempFile.close();

	copyFilesContent("tempFile.csv", fileName);
	return flag;
	}
	

