#include "confientialDataRule.hpp"
#include "filesHandler.hpp"
#include "TestsConfig.hpp"

bool checkIfMaliciousIp(const std::string& dstIp)
{
	//check if the destination IP exists in the malicious IPs file
	return (isStrExistsInFile(maliciousIpsFile, dstIp) != "error");
}

bool checkIfDataConfidential(const std::string& data)
{
	std::vector <std::string> tokens;

	int countTotalWords = 0;
	int countBadWords = 0;
	std::stringstream check(data);

	std::string intermediate;

	// split all the words in the data and insert them to a vector.
	while (getline(check, intermediate, ' '))
	{
		tokens.push_back(intermediate);
	}
	int num = 0;

	//loop through all the words
	for(const std::string& str : tokens)
	{
		//checks if the word is forbidden and if it is then return it's value
		std::string temp = isStrExistsInFile(maliciousWordsFile, str);
		
		countBadWords += atoi(temp.c_str());
		
		
		//checks if the word exists in the list of words the user doesn't allow
		if (isStrExistsInFile(blockWordsFile, str) == "")
			return true;
		countTotalWords++;
	}

	//if the frequanty of bad words is too much bigger then the function will return true
	if (((double)countBadWords / countTotalWords) >= atoi(test_config("FREQ_BAD_WORDS").c_str()))
		return true;
	
	
	return false;

}

