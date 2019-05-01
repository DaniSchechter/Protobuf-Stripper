#include "intervalRule.hpp"
#include "menu.hpp"
#include <chrono>
#include <thread>

int main()
{
	
		std::cout << isInterval("5.5.5.5", "6.6.6.6") + "\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		std::cout << isInterval("5.5.5.5", "6.6.6.6") + "\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		std::cout << isInterval("5.5.5.5", "6.6.6.6") + "\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		std::cout << isInterval("5.5.5.5", "6.6.6.6") + "\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(3000));
		std::cout << isInterval("5.5.5.5", "6.6.6.6") + "\n";
		std::cout << "asdas\n";
		std::this_thread::sleep_for(std::chrono::milliseconds(7000));
		std::cout << isInterval("5.5.5.5", "6.6.6.6") + "\n";

		getchar();
	
	return 0;
}