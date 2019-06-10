#include "checkPacket.hpp"
#include "regexClass.hpp"
#include <fstream>
#include <iostream>

bool is_forbidden(const std::string& src_ip, const std::string& dst_ip, const std::string& dst_port, const std::string& data)
{
	bool is_interval = isInterval(src_ip, dst_ip);
	bool if_data_confidential = checkIfDataConfidential(data);
	bool mal_data = checkIfMaliciousIp(dst_ip);
	bool request_dens = request_density(src_ip, dst_ip, data);
	bool is_encrypted_raw_data = is_encrypted(data);
	bool bport_density = port_density(src_ip, dst_port);
	bool tokens = findTokens(data);

	if (is_interval || if_data_confidential || mal_data || request_dens  || bport_density ) 
	{
		std::cout << request_dens;
		std::cout << is_interval;
		std::cout << if_data_confidential;
		std::cout << mal_data;
		std::cout << bport_density;
		std::cout << tokens;
		
		// TO BLOCK MESSAGE
		writeRequestToFile(src_ip, dst_ip, data);
		std::ofstream file;
		file.open(test_config("MALICIOUS_REQUESTS_FILE"), std::ios::out | std::ios::app);
		file << request_dens;
		file << is_interval;
		file << if_data_confidential;
		file << mal_data;
		file << bport_density;
		file << tokens;
		file.close();
		return true;
	}
	if(tokens)
	{
		//writeRequestToFile(src_ip, dst_ip, data);
	}
	return false;
}
