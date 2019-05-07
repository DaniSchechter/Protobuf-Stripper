#ifndef filesHandler.hpp




#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <sstream>

//The file contains the malicious words with the format: virus-3  
#define maliciousWordsFile "maliciousWords.csv"

//The file contains the words that the user wants to block 
#define blockWordsFile "blockWordsFile.csv"

//The file contains the malicious IPs that need to be blocked
#define maliciousIpsFile "maliciousIps.csv"

//The file contains all the requests that were blocked
#define maliciousRequestsFile "maliciousRequest.csv"


//The function checks if a word exists in a file and if it is then the function return the value
std::string isStrExistsInFile(const std::string& fileName, const std::string& word);

//The fucntion prints the file
void printFile(const std::string& fileName);

//The function copy the content from source file to destination file
void copyFilesContent(const std::string& srcFile, const std::string& dstFile);


//The function delete the lines from the file
bool deleteLinesFromFile(const std::string& fileName, std::set<std::string> s);

#endif // !filesHandler.hpp