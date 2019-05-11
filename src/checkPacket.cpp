#include "checkPacket.hpp"

bool isForbidden(const std::string& srcIP, const std::string& dstIP, const std::string& data)
{
	
	//checks if the packet was sent in the same frequanty
	//checks if the data contains unallowed words
	//checks if the packet was sent to a malicious IP
	if (isInterval(srcIP, dstIP) || checkIfDataConfidential(data) || checkIfMaliciousIp(dstIP))
	{
		// TO BLOCK MESSAGE
		writeRequestToFile(srcIP, dstIP, data);
		return true;
	}
	return false;
}
