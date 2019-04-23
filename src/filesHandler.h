

#include <iostream> 
#include <stdio.h>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

#define maliciousWordsFile "maliciousWords.csv"
#define blockWordsFile "blockWordsFile.csv"
#define maliciousIpsFile "maliciousIps.csv"
#define maliciousRequestsFile "maliciousRequest.csv"



std::string isStrExistsInFile(const std::string& fileName, const std::string& word);

void printFile(const std::string& fileName);

void copyFilesContent(const std::string& srcFile, const std::string& dstFile);

bool deleteLinesFromFile(const std::string& fileName, std::vector<std::string> vec);