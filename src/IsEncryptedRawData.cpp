#include "IsEncryptedRawData.hpp"
//check if the raw data of unencrypted protocol is encrypted by purpose.
bool Is_Encrypted(const std::string& rawdata)
{
	//high entropy means more chances the raw data is encrypted
	return Entropy(rawdata) > std::stoi(test_config("Min_Entropy"))
}