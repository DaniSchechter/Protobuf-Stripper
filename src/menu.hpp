

#include "filesHandler.hpp"



#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
#include <set>


#define menu_size 8

//The function gets a packet and print in to the bad requests file
void writeRequestToFile(const std::string& srcIP, const std::string& dstIP, const std::string& data);

//The function checks if the IP is valid
bool isValidIp(const std::string& ip);

//The function add IPs to the bad IPs list
void setMaliciousIp();

//The function prints the bad requests
void viewBadRequests();

//The function add words to the block words list
void setBadWords();

//The function gets the wanted actions from user 
void menu();

//The function prints the bad words list
void showBadWordsList();

//The function returns the current time
const std::string currentDateTime();

//The function gets which words to delete
std::set<std::string> chooseDelete();

//The function prints the malicious IPs list
void showBadIpsList();

//The function checks if the number is valid
bool isValidNumber(int num);

//The function checks if the word is valid
bool isValidWord(const std::string& word);

//The function changes the bad words list
void changeBadWordsList();

//The function changes the bad IPs list
void changeBadIpsList();
