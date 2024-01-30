#include<iostream>
#include<string>

int main()
{
	using namespace std;
	
	const string Month[12]={"Jan","Feb","Mar","Apr",
						  "May","Jun","Jul","Aug",
						  "Sep","Oct","Nov","Dec"};
	int sale[12];
	int sum=0;
	for(int i=0;i<12;i++){
		cout<<"Enter the sale number of "<<Month[i]<<": ";
		cin>>sale[i];
		sum+=sale[i];
	}
	cout<<"Input Done!"<<endl;
	cout<<"Total sale: "<<sum<<endl;
	
	return 0;
}
