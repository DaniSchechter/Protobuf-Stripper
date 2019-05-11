#include "menu.hpp"


const std::string currentDateTime()
{
	time_t     now = time(0);
	struct tm  tstruct;
	char       buf[80];

	localtime_s(&tstruct, &now);
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

bool isValidNumber(int num)
{
	if (num > 0 && num <= menu_size)
		return true;
	return false;
}

bool isValidIp(const std::string& ip)
{
	char * charIp = new char[ip.length() + 1];
	try
	{
		
		int digitsAmount = 0;
		strcpy_s(charIp, ip.length() + 1, ip.c_str());
		char* context = NULL;
		char* words = strtok_s(charIp, ".", &context);
		int count = 0;
		
		//parsing the segments of the IP
		while (words != NULL)
		{
			int size = strlen(words);
			
			//check if the ip segment contains only numbers
			for (char i = 0; i < size; i++)
			{
				if (words[i] - '0' > 9 || words[i] - '0' < 0)
				{
					delete[] charIp;
					return false;
				}
				else
					digitsAmount += 1;
			}
			//if the length of the segment is bigger than 3
			if (size > 3)
			{
				delete[] charIp;
				return false;
			}
			count++;
			words = strtok_s(NULL, ".", &context);
		}
		//check if there is 4 segments and 3 dots.
		if (count == 4 && ip.length() - digitsAmount == 3)
		{
			delete[] charIp;
			return true;
		}
		delete[] charIp;
		return false;
	}
	catch (char *e)
	{
		return false;
	}


}

void setMaliciousIp()
{
	std::cout << "\n\nPlease enter the bad ip" << '\n';
	std::cout << "To stop please press 99" << '\n';

	std::string ip;
	std::ofstream file;
	file.open(maliciousIpsFile, std::ios::out | std::ios::app);
	do
	{
		std::cin >> ip;
		
		//check if the IP in valid
		if (isValidIp(ip) == true)
			file << ip + "\n";
		else if (ip != "99")
			std::cout << "Please enter a valid ip" << '\n';

	} while (ip != "99");
	file.close();
}

bool isValidWord(const std::string& word)
{
	int len = word.length();
	if (word == "")
		return false;
	
	//check if the word doesnt contain digits.
	for (int i = 0; i < len; i++)
	{
		if (word[i] - '0' <= 9 && word[i] - '0' >= 0)
			return false;
	}
	return true;
}

void setBadWords()
{

	std::cout << "\n\nPlease enter the bad word" << '\n';
	std::cout << "To stop please press 99" << '\n';

	std::string word;
	std::ofstream file;
	file.open(blockWordsFile, std::ios::out | std::ios::app);
	do
	{
		std::cin >> word;
		if (isValidWord(word) == true)
			file << word + "\n";
		else if (word != "99")
			std::cout << "Please enter a valid word" << '\n';

	} while (word != "99");

	file.close();
}

void viewBadRequests()
{
	std::fstream file;
	file.open(maliciousRequestsFile, std::ios::in);
	std::ifstream f(maliciousRequestsFile);
	std::string line;
	std::cout << "\n\n";
	
	//check if the file opened
	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file, line);

			std::cout << line + '\n';
		}
		file.close();
	}
	//if the file isn't exist
	else if (f.good() == false)
		std::cout << "There aren't bad requests\n";
	
	//the file exists but can't be opened
	else
		std::cout << "There was an error opening the file\n";
}

void menu()
{
	int num;

	std::cout << "MENU:" << '\n';

	std::cout << "Please choose from the options below:" << '\n';

	std::cout << "1. enter a malicious ip to block access" << '\n';

	std::cout << "2. enter a malicious words to block data transfer" << '\n';

	std::cout << "3. view all the bad requests" << '\n';

	std::cout << "4. view all the bad IPs" << '\n';

	std::cout << "5. view all the bad words" << '\n';

	std::cout << "6. change the bad IPs list" << '\n';

	std::cout << "7. change the bad words list" << '\n';

	std::cout << "8. To do all the options above" << '\n';

	std::cout << "To stop please press 99" << '\n';

	int options[menu_size];
	bool flag = false;
	std::cin.exceptions(std::istream::failbit);
	
	//getting the wanted actions from the user
	do
	{

		try
		{
			std::cin.clear();
			std::cin >> num;
			flag = true;
			if (num == menu_size)
			{
				flag = false;
				options[menu_size - 1] = 1;
				break;
			}
			if (isValidNumber(num) == true)
			{
				flag = false;
				options[num - 1] = 1;
			}
			else if (num != 99)
			{
				std::cout << "Please enter a valid choice\n";
				flag = true;
			}
		}
		catch (std::ios_base::failure &fail)
		{

			std::cout << "Not a valid input\n";
			std::cin.clear();
			std::cin.ignore();
			flag = false;
		}

	} while (num != 99 && flag == false);


	if (options[menu_size - 1] == 1)
	{
		setMaliciousIp();
		setBadWords();
		viewBadRequests();
		showBadIpsList();
		showBadWordsList();
		changeBadIpsList();
		changeBadWordsList();
	}
	else
	{
		if (options[0] == 1)
			setMaliciousIp();
		if (options[1] == 1)
			setBadWords();
		if (options[2] == 1)
			viewBadRequests();
		if (options[3] == 1)
			showBadIpsList();
		if (options[4] == 1)
			showBadWordsList();
		if (options[5] == 1)
			changeBadIpsList();
		if (options[6] == 1)
			changeBadWordsList();
	}

}

void showBadWordsList()
{
	std::cout << "\n\nThe Bad Words Are:" << '\n';

	printFile(blockWordsFile);

}

void showBadIpsList()
{
	std::cout << "\n\nThe Bad IPs Are:" << '\n';

	printFile(maliciousIpsFile);

}


void writeRequestToFile(const std::string& srcIP, const std::string& dstIP, const std::string& data)
{
	std::ofstream file;
	file.open(maliciousRequestsFile, std::ios::out | std::ios::app);

	file << "---------------------------------\n\n";
	file << srcIP << ", " << dstIP << '\n';
	file << data << '\n';
	file << currentDateTime() << "\n\n";
	file.close();
}


std::set<std::string> chooseDelete()
{

	std::set<std::string> s;
	std::string str;

	std::cout << "\n\nPlease enter what you want to delete\n";
	std::cout << "To stop please insert 99\n";
	
	//getting from the user which words to delete
	do
	{
		std::cin >> str;
		if (str != "99")
			s.insert(s.begin(), str);
	} while (str != "99");

	return s;
}

void changeBadWordsList()
{
	showBadWordsList();

	if (deleteLinesFromFile(blockWordsFile, chooseDelete()))
		std::cout << "\nThe list was updated\n";
	else
		std::cout << "\nThere were noting to update\n";
}

void changeBadIpsList()
{
	showBadIpsList();
	if (deleteLinesFromFile(maliciousIpsFile, chooseDelete()))
		std::cout << "\nThe list was updated\n";
	else
		std::cout << "\nThere were noting to update\n";
}

