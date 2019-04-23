#include "filesHandler.h"

//To open the files before the functions and to pass the text in a string variable.
//To call data from disk as little as possible.(efficiency)

std::string isStrExistsInFile(const std::string& fileName, const std::string& word)
{

	std::fstream file;

	file.open(fileName, std::ios::in);
	if (file.is_open())
	{
		std::string line;
		char* parseLine;

		while (!file.eof())
		{
			getline(file, line);

			if (strcmp(line.c_str(), "") != 0)
			{

				std::vector <std::string> tokens;


				std::stringstream split(line);

				std::string intermediate;


				while (getline(split, intermediate, '-'))
				{
					tokens.push_back(intermediate);
				}
				if (strcmp(tokens[0].c_str(), word.c_str()) == 0)
				{
					file.close();
					if (tokens.size() == 2)
						return tokens[1];
					else
						return "";
				}
			}
		}
		file.close();

	}
	return "0";
}


void printFile(const std::string& fileName)
{
	std::fstream file;

	file.open(fileName, std::ios::in);
	if (file.is_open())
	{
		std::string line;

		while (!file.eof())
		{
			getline(file, line);
			std::cout << line << '\n';
		}
		file.close();
	}

}

void copyFilesContent(const std::string& srcFile, const std::string& dstFile)
{
	remove(dstFile.c_str());

	std::string line;
	std::ofstream file;
	file.open(dstFile, std::ios::out | std::ios::app);

	std::fstream temp;

	temp.open(srcFile, std::ios::in);
	if (temp.is_open())
	{
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
}

bool deleteLinesFromFile(const std::string& fileName, std::vector<std::string> vec)
{

	std::fstream file;
	bool flag = false;

	file.open(fileName, std::ios::in);
	if (file.is_open())
	{
		std::string line;

		std::ofstream tempFile;
		tempFile.open("tempFile.csv");

		if (vec.size() == 0)
			return false;
		while (!file.eof())
		{
			getline(file, line);

			if (std::find(vec.begin(), vec.end(), line) == vec.end() && strcmp(line.c_str(), "\r\n") != 0)
				tempFile << line + "\n";
			else if (line != "")
				flag = true;

		}
		file.close();
		tempFile.close();

		copyFilesContent("tempFile.csv", fileName);
		return flag;
	}
	return flag;
}