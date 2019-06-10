#ifndef FILES_HANDLER_HPP_

#define FILES_HANDLER_HPP_

#include <string>
#include <set>

#define ERROR_MESSAGE "error opening the file"

#define BLOCK_WORD -1

#define BAD_IP -1

//The function checks if a word exists in a file and if it is then the function return the value
int isStrExistsInFile(const std::string& fileName, const std::string& word);

//The fucntion prints the file
void printFile(const std::string& fileName);

//The function copy the content from source file to destination file
void copyFilesContent(const std::string& srcFile, const std::string& dstFile);


//The function delete the lines from the file
bool deleteLinesFromFile(const std::string& fileName, std::set<std::string> s);

#endif // !filesHandler.hpp