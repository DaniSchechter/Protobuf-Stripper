

#include "requestInterval.h"
#include "hashOverride.h"


#pragma once
#include <iostream> 
#include <unordered_map> 
#include <stdio.h>
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>



long calculateDuration();


bool isInterval(const std::string& srcIP, const std::string& dstIP);

