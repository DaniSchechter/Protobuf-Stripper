#include "confientialDataRule.hpp"
#include "filesHandler.hpp"
#include "TestsConfig.hpp"
#include <vector>
#include <sstream>
#include <cstdlib>
#include <iostream> 


bool checkIfMaliciousIp(const std::string& dstIp)
{
	//check if the destination IP exists in the malicious IPs file
	return (isStrExistsInFile(MALICIOUS_IP_FILE, dstIp) == BAD_IP);
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
		countBadWords += isStrExistsInFile(MALICIOUS_WORDS_FILE, str);
		
		
		//checks if the word exists in the list of words the user doesn't allow
		if (isStrExistsInFile(BLOCK_WORDS_FILE, str) == BLOCK_WORD)
			return true;
		countTotalWords++;
	}

	//if the frequanty of bad words is too much bigger then the function will return true
	if (((double)countBadWords / countTotalWords) >= std::stoi(test_config("FREQ_BAD_WORDS")))
		return true;
	
	
	return false;

}

