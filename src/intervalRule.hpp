#ifndef INTERVAL_RULE_HPP_	

#define INTERVAL_RULE_HPP_


#include <string>


#define INTERVAL_ERROR 500

//The function returns the number of miliseconds since epoch time
long calculateDuration();

// The function checks if the packet was sent with the same frequanty
bool isInterval(const std::string& srcIP, const std::string& dstIP);

#endif