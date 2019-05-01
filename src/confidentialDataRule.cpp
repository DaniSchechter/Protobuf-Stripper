#include "confientialDataRule.hpp"
#include "filesHandler.hpp"

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
	for (int i = 0; i < tokens.size(); i++)
	{
		//checks if the word is forbidden and if it is then return it's value
		std::string temp = isStrExistsInFile(maliciousWordsFile, tokens[i]);
		
		countBadWords += atoi(temp.c_str());
		
		
		//checks if the word exists in the list of words the user doesn't allow
		if (isStrExistsInFile(blockWordsFile, tokens[i]) == "")
			return true;
		countTotalWords++;
	}


	//if the data is smaller than 1000 words and the number of bad words are greater than 5 then the packet is forbidden
	if (countTotalWords <= 1000 && countBadWords >= 5)
		return true;
	
	//like the previous comment
	else if (countTotalWords >= 1000 && countBadWords >= 8)
		return true;
	return false;

}

