#include "checkPacket.hpp"

bool isForbidden(const std::string& srcIP, const std::string& dstIP, const std::string& srcPort, const std::string& dstPort, const std::string& data)
{
	bool is_interval = isInterval(srcIP, dstIP);
	bool if_data_confidential = checkIfDataConfidential(data);
	bool mal_data = checkIfMaliciousIp(dstIP);
	bool request_dens = request_density(srcIP, dstIP, data);
	bool is_encrypted = Is_Encrypted(data);
	bool bport_density = port_density(srcIP, dstIP, srcPort, dstPort);

	if (is_interval || if_data_confidential || mal_data || request_dens || is_encrypted || bport_density) 
	{
		// TO BLOCK MESSAGE
		writeRequestToFile(srcIP, dstIP, data);
		return true;
	}
	return false;
}
