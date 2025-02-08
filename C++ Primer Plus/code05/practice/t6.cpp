#include<iostream>
#include<string>

int main()
{
	using namespace std;
	
	const string Month[12]={"Jan","Feb","Mar","Apr",
							"May","Jun","Jul","Aug",
							"Sep","Oct","Nov","Dec"};
	int sale[3][12];
	int sum=0;
	for(int i=0;i<3;i++){
		cout<<"Starting "<<i+1<<" year's data"<<endl;
		for(int j=0;j<12;j++){
			cout<<"Enter the sale number of "<<Month[j]<<": ";
			cin>>sale[i][j];
		}
	}
	cout<<"Input Done!"<<endl;
	int persum;
	for(int i=0;i<3;i++){
		persum=0;
		for(int j=0;j<12;j++){
			persum+=sale[i][j];
		}
		sum+=persum;
		cout<<i+1<<" year total sale: "<<persum<<endl;
	}
	cout<<"Three years total sale: "<<sum<<endl;
	
	return 0;
}
