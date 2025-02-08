#ifndef __ACCOUNT_H__
#define __ACCOUNT_H__

#include <string>

class Account
{
	private:
		std::string name;
		std::string number;
		double balance;
	public:
		Account(const std::string client,const std::string num,double bal=0.0);
		void show() const;
		void deposit(double cash);
		void withdraw(double cash);
};

#endif
