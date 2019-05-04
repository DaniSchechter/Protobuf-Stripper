#include "IsEncryptedRawData.hpp"
//check if the raw data of unencrypted protocol is encrypted by purpose.
bool Is_Encrypted(const std::string& rawdata)
{
	//high entropy means more chances the raw data is encrypted
	if (Entropy(rawdata) > atoi(test_config("Min_Entropy").c_str()))
		return true;
	return false;
}