#include <iostream>
#include "account.h"

int main()
{
	Account n1("Rick","0001",1000);
	std::cout<<"The account information: "<<std::endl;
	n1.show();

	std::cout<<std::endl;
	n1.deposit(500);
	std::cout<<"After deposit $500: "<<std::endl;
	n1.show();

	std::cout<<std::endl;
	n1.withdraw(800);
	std::cout<<"After withdraw $800: "<<std::endl;
	n1.show();

	return 0;
}
