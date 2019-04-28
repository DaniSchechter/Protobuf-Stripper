
#include <iostream>
#include <unordered_map>
#include <stdio.h>
#include <ctime>
#include <string>
#include <set>

class destinationIp
{
private:
	std::string dstIp;
public:
	void setDstIp(std::string ip)
	{
		this->dstIp = ip;
	}
	std::string getDstIp() const
	{
		return this->dstIp;
	}
	destinationIp(std::string dstIp)
	{
		this->dstIp = dstIp;
	}
	bool operator==(destinationIp const& other) const {
		if (this->getDstIp() == other.getDstIp())
			return true;
		return false;
	}
};
bool request_density(const std::string& srcIP, const std::string& dstIP, const std::string& rawdata);