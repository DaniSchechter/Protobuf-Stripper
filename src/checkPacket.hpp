#ifndef checkPacket.hpp

#include "intervalRule.hpp"
#include "confientialDataRule.hpp"
#include "menu.hpp"
#include <string>
#include "PortsTest.hpp"
#include "IpBlackList.hpp"
#include "IsEncryptedRawData.hpp"

//The function calls all of the rules and return true if the message isn't valid
bool isForbidden(const std::string& srcIP, const std::string& dstIP, const std::string& srcPort, const std::string& dstPort, const std::string& data);
#endif // !checkPacket.hpp