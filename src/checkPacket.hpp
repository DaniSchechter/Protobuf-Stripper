#ifndef CHECK_PACKET_HPP_

#define CHECK_PACKET_HPP_

#include "intervalRule.hpp"
#include "confientialDataRule.hpp"
#include "menu.hpp"
#include <string>
#include <iostream>
#include "PortsTest.hpp"
#include "IpBlackList.hpp"
#include "IsEncryptedRawData.hpp"

//The function calls all of the rules and return true if the message isn't valid
bool is_forbidden(const std::string& srcIP, const std::string& dstIP, const std::string& srcPort, const std::string& dstPort, const std::string& data);
#endif // !checkPacket.hpp