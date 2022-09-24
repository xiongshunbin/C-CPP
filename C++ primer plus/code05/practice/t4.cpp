#include<iostream>
const int Deposit=100;

int main()
{
	using namespace std;
	double Daphne=Deposit;
	double Cleo=Deposit;
	int year=0;
	
	while(Daphne>=Cleo)
	{
		Daphne+=0.1*Deposit;
		Cleo+=0.05*Cleo;
		year++;
	}
	cout<<"After "<<year<<" years, Cleo has more money than Daphne!"<<endl;
	cout<<"Daphne has "<<Daphne<<"$."<<endl;
	cout<<"Cleo has "<<Cleo<<"$."<<endl;
	
	return 0;
}
