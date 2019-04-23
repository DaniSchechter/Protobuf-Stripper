

#include "intervalRule.h"
#include "confientialDataRule.h"
#include "menu.h"
#include <string>
#include <string.h>
#include <stdio.h>

bool isForbidden(const std::string& srcIP, const std::string& dstIP, const std::string& data);