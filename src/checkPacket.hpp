

#include "intervalRule.hpp"
#include "confientialDataRule.hpp"
#include "menu.hpp"
#include <string>

//The function calls all of the rules and return true if the message isn't valid
bool isForbidden(const std::string& srcIP, const std::string& dstIP, const std::string& data);
