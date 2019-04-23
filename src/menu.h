

#include "filesHandler.h"

#include <iostream> 
#include <unordered_map> 
#include <stdio.h>
#include <ctime>
#include <chrono>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


#define menu_size 8


void writeRequestToFile(const std::string& srcIP, const std::string& dstIP, const std::string& data);
bool isValidIp(const std::string& ip);

void setMaliciousIp();

void viewBadRequests();

void setBadWords();

void menu();


void showBadWordsList();
const std::string currentDateTime();

std::vector<std::string> chooseDelete();

void showBadIpsList();

bool isValidNumber(int num);

bool isValidWord(const std::string& word);

void changeBadWordsList();

void changeBadIpsList();