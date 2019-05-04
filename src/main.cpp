#include "EncryptedData.hpp"
#include <iostream>
#include <string>
using namespace std;

int main()
{

	string strtest = "";
	cout << "Plain text entropy: " << Entropy(strtest)<< "\n";
	string strteste = "";
	cout << "encrypted text entropy: " << Entropy(strteste);
	return 0;
}