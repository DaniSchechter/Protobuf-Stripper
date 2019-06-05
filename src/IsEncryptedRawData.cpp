#include "IsEncryptedRawData.hpp"
#include "TestsConfig.hpp"
//check if the raw data of unencrypted protocol is encrypted by purpose.
bool is_encrypted(const std::string& rawdata)
{
	//high entropy means more chances the raw data is encrypted
	if(rawdata.size() < stoi(test_config("ENCRYPTED_RAW_DATA_MIN_SIZE")))
		return false;
	return Entropy(rawdata) > std::stoi(test_config("Min_Entropy"));
	
}