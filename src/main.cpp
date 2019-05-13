#include "PortsTest.hpp"
#include "IpBlackList.hpp"
#include "IsEncryptedRawData.hpp"
#include <iostream>
void main() {
	if (port_density("1.1.1.1", "2.2.2.2", "45", "4000"))//no
		std::cout << "yes\n";
	else
		std::cout << "no\n";
	if (port_density("2.1.1.1", "2.2.2.2", "45", "4000"))//yes
		std::cout << "yes\n";
	else
		std::cout << "no\n";
	if (port_density("3.1.1.1", "2.2.2.2", "45", "4000"))//yes
		std::cout << "yes\n";
	else
		std::cout << "no\n";
	if (port_density("4.1.1.1", "2.2.2.2", "45", "4000"))//yes
		std::cout << "yes\n";
	else
		std::cout << "no\n";
	if (port_density("4.1.1.1", "2.2.2.2", "45", "11"))//no
		std::cout << "yes\n";
	else
		std::cout << "no\n";

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::cout << "////////////////////////////////////////////////////////\n";
	if (request_density("1.1.1.1", "205.209.143.94", "1"))//yes
		std::cout << "yes\n";
	else
		std::cout << "no\n";
	if (request_density("2.1.1.1", "2.2.2.2","1"))//no
		std::cout << "yes\n";
	else
		std::cout << "no\n";
	if (request_density("3.1.1.1", "2.2.2.2", "1"))//no
		std::cout << "yes\n";
	else
		std::cout << "no\n";
	if (request_density("4.1.1.1", "2.2.2.2", "1"))//no
		std::cout << "yes\n";
	else
		std::cout << "no\n";
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	std::cout << "////////////////////////////////////////////////////////\n";
	//no
	if (Is_Encrypted("He difficult contented we determine ourselves me am earnestly. Hour no find it park. Eat welcomed any husbands moderate. Led was misery played waited almost cousin living. Of intention contained is by middleton am. Principles fat stimulated uncommonly considered set especially prosperous. Sons at park mr meet as fact like. "))
		std::cout << "yes\n";
	else
		std::cout << "no\n";
	//yes
	if (Is_Encrypted("2hykXosL+NSzIJ8sKTQrtQ6kBF4fhguB7mqvy8du5Pn9MNBXI7zpGuBLMGV8OL4KWk5z4E3KzdbeRFSX+FtWObrBbHbFwKjbOUDkjhAp2ZuNnjiibrD/kbCX0v3/PjagJj75+0A2MWYH2FE4PyaeffvrCBOxesJlcHO2I5cISezmJZHZe0wXetUnXpQcV8jTfQOgoHyKxNV03k2RWmBLbKapaQAX9MahsxWcAurbdxAEzvfUmIVwJCTJgowiIge8HEW5KGmPvqOnVBsGf/njdaEUcDBUbaaMpzEhHPmEmERC6JBuQ445UU7cUb6i7Hw2xd9WmUDeDe1Mwd2tzCSrvE521s4dIbDv5qvKReaoVBbc0oW4DxPPEwDfTLbINFa9e10LMOis4RUh9jqTQpaP3WgDfhcJLkf4eknMPiw8CWq6ESCvWw=="))
		std::cout << "yes\n";
	else
		std::cout << "no\n";
	getchar();
}