#include "checkPacket.hpp"
#include "regexClass.hpp"

bool is_forbidden(const std::string& src_ip, const std::string& dst_ip, const std::string& dst_port, const std::string& data)
{
	bool is_interval = isInterval(src_ip, dst_ip);
	bool if_data_confidential = checkIfDataConfidential(data);
	bool mal_data = checkIfMaliciousIp(dst_ip);
	bool request_dens = request_density(src_ip, dst_ip, data);
	bool is_encrypted_raw_data = is_encrypted(data);
	bool bport_density = port_density(src_ip, dst_port);
	bool tokens = findTokens(data);

	if (is_interval || if_data_confidential || mal_data || request_dens || is_encrypted_raw_data || bport_density || tokens ) 
	{
		std::cout << request_dens;
		// TO BLOCK MESSAGE
		writeRequestToFile(src_ip, dst_ip, data);
		return true;
	}
	return false;
}
