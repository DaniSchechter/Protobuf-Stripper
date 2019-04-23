#include "confientialDataRule.h"
#include "filesHandler.h"

bool checkIfMaliciousIp(const std::string& dstIp)
{
	if (isStrExistsInFile(maliciousIpsFile, dstIp) != "0")
		return true;
	return false;
}

bool checkIfDataConfidential(const std::string& data)
{
	std::vector <std::string> tokens;

	int countTotalWords = 0;
	int countBadWords = 0;
	std::stringstream check(data);

	std::string intermediate;


	while (getline(check, intermediate, ' '))
	{
		tokens.push_back(intermediate);
	}
	int num = 0;

	for (int i = 0; i < tokens.size(); i++)
	{
		std::string temp = isStrExistsInFile(maliciousWordsFile, tokens[i]);
		if (strcmp(temp.c_str(), "0") != 0)
		{
			std::stringstream strToInt(temp);
			strToInt >> num;
			countBadWords += num;
		}
		if (isStrExistsInFile(blockWordsFile, tokens[i]) == "")
			return true;
		countTotalWords++;
	}



	if (countTotalWords <= 1000 && countBadWords >= 5)
		return true;
	else if (countTotalWords >= 1000 && countBadWords >= 8)
		return true;
	return false;

}

