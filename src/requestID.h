#ifndef requestID.h


#endif

class requestID
{
private:
	std::string srcIp;
	std::string dstIp;

public:
	requestID()
	{
		this->srcIp = "";
		this->dstIp = "";
	}
	requestID(const std::string& srcip, const std::string& dstip)
	{
		this->srcIp = srcip;
		this->dstIp = dstip;
	}
	void setSrcIp(const std::string& ip)
	{
		this->srcIp = ip;
	}
	void setDstIp(const std::string& ip)
	{
		this->dstIp = ip;
	}
	std::string getSrcIp()const
	{
		return this->srcIp;
	}
	std::string getDstIp()const
	{
		return this->dstIp;
	}
	bool operator==(requestID const& other) const {
		if (this->getSrcIp() == other.getSrcIp() && this->getDstIp() == other.getDstIp())
			return true;
		return false;
	}
};
















