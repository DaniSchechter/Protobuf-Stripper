#include <iostream>
#include <unordered_map>
#include <stdio.h>
#include <ctime>
#include <string>

class requestID
{
private:
	std::string srcIp;
	std::string dstIp;

public:
	void setSrcIp(std::string ip)
	{
		this->srcIp = ip;
	}
	void setDstIp(std::string ip)
	{
		this->dstIp = ip;
	}
	std::string getSrcIp()
	{
		return this->srcIp;
	}
	std::string getDstIp()
	{
		return this->dstIp;
	}
};

class requestInterval
{
private:
	std::time_t recieveTime;
	std::time_t interval;

public:
	void setRecieveTime(std::time_t ip)
	{
		this->recieveTime = ip;
	}
	void setInterval(std::time_t ip)
	{
		this->interval = ip;
	}
	std::time_t getRecieveTime()
	{
		return this->recieveTime;
	}
	std::time_t getInterval()
	{
		return this->interval;
	}
};

class destinationIp
{
private:
	std::string dstIp;
public:
	void setDstIp(std::string ip)
	{
		this->dstIp = ip;
	}
	std::string getDstIp()
	{
		return this->dstIp;
	}
	destinationIp(std::string dstIp)
	{
		this->dstIp = dstIp;
	}
};
bool requestdensity(std::string srcIP, std::string dstIP, std::string rawdata);