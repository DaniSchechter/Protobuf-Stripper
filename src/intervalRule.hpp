#ifndef intervalRule.hpp

#include "requestInterval.hpp"
#include "hashOverride.hpp"
#include <iostream>
#include <unordered_map> 
#include <ctime>
#include <chrono>
#include <string>


#define intervalError 500

//The function returns the number of miliseconds since epoch time
long calculateDuration();

// The function checks if the packet was sent with the same frequanty
bool isInterval(const std::string& srcIP, const std::string& dstIP);

#endif