#include<iostream>

using namespace std;

void print(int,int);
int main()
{
	int hours;
	int minutes;
	
	cout<<"Enter the number of hours: ";
	cin>>hours;
	cout<<"Enter the number of minutes: ";
	cin>>minutes;
	print(hours,minutes);
	return 0;
}
void print(int hours,int minutes)
{
	cout<<"Time "<<hours<<":"<<minutes;
}
