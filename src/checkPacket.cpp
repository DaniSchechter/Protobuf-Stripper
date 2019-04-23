#include "checkPacket.h"


bool isForbidden(const std::string& srcIP, const std::string& dstIP, const std::string& data)
{
	bool flag = false;
	if (isInterval(srcIP, dstIP) == true)
	{
		// TO BLOCK MESSAGE
		flag = true;
	}
	else if (checkIfDataConfidential(data) == true)
	{
		// TO BLOCK MESSAGE
		flag = true;
	}
	else if (checkIfMaliciousIp(dstIP) == true)
	{
		// TO BLOCK MESSAGE
		flag = true;
	}


	if (flag == true)
	{
		writeRequestToFile(srcIP, dstIP, data);
		return true;
	}
	return false;
}