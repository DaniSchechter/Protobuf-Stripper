#ifndef CONFIDENTIAL_DATA_RULE_HPP_
#define CONFIDENTIAL_DATA_RULE_HPP_
#include <string>
//The function checks if the IP is defined as a malicious
bool checkIfMaliciousIp(const std::string& dstIp);

//The function checks if the data is consider as a malicious
bool checkIfDataConfidential(const std::string& data);

#endif