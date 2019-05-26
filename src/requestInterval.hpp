#ifndef requestInterval.h

#include <ctime>
#include <chrono>



class requestInterval
{

private:
	std::time_t recieveTime;
	std::time_t interval;

public:
	requestInterval()
	{
	}
	requestInterval(std::time_t recievetime, std::time_t interval)
	{
		this->recieveTime = recievetime;
		this->interval = interval;

	}
	void setRecieveTime(std::time_t ip)
	{
		this->recieveTime = ip;
	}
	void setInterval(std::time_t ip)
	{
		this->interval = ip;
	}
	std::time_t getRecieveTime()const
	{
		return this->recieveTime;
	}
	std::time_t getInterval()const
	{
		return this->interval;
	}
};

#endif