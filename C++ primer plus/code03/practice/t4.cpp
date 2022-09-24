#include<iostream>
int main()
{
	using namespace std;
	const int DATY_HOUR=24;
	const int HOUR_MINUTE=60;
	const int MINUTE_SECOND=60;
	
	long long seconds,temp;;
	int days,hours,minutes;
	
	cout<<"Enter the number of seconds: ";
	cin>>seconds;
	temp=seconds;
	days=seconds/(DATY_HOUR*HOUR_MINUTE*MINUTE_SECOND);
	seconds=seconds%(DATY_HOUR*HOUR_MINUTE*MINUTE_SECOND);
	hours=seconds/(MINUTE_SECOND*HOUR_MINUTE);
	seconds=seconds%(MINUTE_SECOND*HOUR_MINUTE);
	minutes=seconds/(MINUTE_SECOND);
	seconds=seconds%(MINUTE_SECOND);
	cout<<temp<<" seconds = "
		<<days<<" days, "
		<<hours<<" hours, "
		<<minutes<<" minutes, "
		<<seconds<<" seconds";
	
	return 0;
}
