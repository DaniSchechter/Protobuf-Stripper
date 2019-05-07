#ifndef confientialDataRule.hpp
#include <iostream> 
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>


//The function checks if the IP is defined as a malicious
bool checkIfMaliciousIp(const std::string& dstIp);

//The function checks if the data is consider as a malicious
bool checkIfDataConfidential(const std::string& data);

#endif