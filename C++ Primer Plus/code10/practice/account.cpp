#include <iostream>
#include "account.h"

Account::Account(const std::string client,const std::string num,double bal)
{
	name=client;
	number=num;
	balance=bal;
}

void Account::show() const
{
	std::cout<<"name: "<<name<<" number: "<<number
		 <<" balance: $"<<balance<<std::endl;
}

void Account::deposit(double cash)
{
	balance+=cash;
}

void Account::withdraw(double cash)
{
	balance-=cash;
}


