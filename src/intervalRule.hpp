

#include "requestInterval.hpp"
#include "hashOverride.hpp"


#pragma once

#include <iostream>
#include <unordered_map> 
#include <ctime>
#include <chrono>
#include <string>



//The function returns the number of miliseconds since epoch time
long calculateDuration();

// The function checks if the packet was sent with the same frequanty
bool isInterval(const std::string& srcIP, const std::string& dstIP);

