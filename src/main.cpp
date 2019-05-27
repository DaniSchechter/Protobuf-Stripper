#include "intervalRule.hpp"
#include "menu.hpp"
#include <chrono>
#include <thread>
#include <iostream>
#include "checkPacket.hpp"




int main()
{

	auto start = std::chrono::high_resolution_clock::now();
	isForbidden("5.5.5.55", "1.1.1.11", "this is good");
	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	std::cout << duration.count() << "\n";


	

	getchar();
	

	return 0;
}