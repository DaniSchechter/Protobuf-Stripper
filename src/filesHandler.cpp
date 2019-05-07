#include "filesHandler.hpp"

//To open the files before the functions and to pass the text in a string variable.
//To call data from disk as little as possible.(efficiency)

std::string isStrExistsInFile(const std::string& fileName, const std::string& word)
{

	std::fstream file;

	file.open(fileName, std::ios::in);
	std::vector<std::string> tokens;
	//checks if the file opened properlly
	if (!file.is_open())
		return "error";
	std::string line;
	char* parseLine;

	while (!file.eof())
	{
		//get a line from the file
		getline(file, line);

		if (strcmp(line.c_str(), "") != 0)
		{




			std::stringstream split(line);

			std::string intermediate;

			//split the line from the value (ex.   virus-3)
			while (getline(split, intermediate, '-'))
			{
				tokens.push_back(intermediate);
			}

			//if the word from the file is the same as the checked word
			if (strcmp(tokens[0].c_str(), word.c_str()) == 0)
			{
				file.close();

				// if the size if 2 then there is a value in the file (the function will call with two types of files)
				if (tokens.size() == 2)
					return tokens[1];
				else
					return "";
			}
			std::vector<std::string> tokens;
		}
		std::vector<std::string> tokens;
	}
	file.close();

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
		if (strcmp(line.c_str(), "\r\n") != 0 && strcmp(line.c_str(), "") != 0)
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
		if (std::find(s.begin(), s.end(), line) == s.end() && strcmp(line.c_str(), "\r\n") != 0)
			tempFile << line + "\n";
		else if (line != "")
			flag = true;

	}
	file.close();
	tempFile.close();

	copyFilesContent("tempFile.csv", fileName);
	return flag;
	}
	
