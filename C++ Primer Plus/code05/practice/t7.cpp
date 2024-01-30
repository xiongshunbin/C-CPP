#include<iostream>
#include<string>

using namespace std;

struct car
{
	string producer;
	int date;
};

int main()
{
	int num;
	cout<<"How many cars do you wish to catalog? ";
	(cin>>num).get();
	car* pcar=new car[num];
	for(int i=0;i<num;i++)
	{
		cout<<"Car #"<<i+1<<":\n";
		cout<<"Please enter the maker: ";
		getline(cin,pcar[i].producer);
		cout<<"Please enter the year made: ";
		(cin>>pcar[i].date).get();
	}
	cout<<"Here is your collection:\n";
	for(int i=0;i<num;i++)
		cout<<pcar[i].date<<" "<<pcar[i].producer<<endl;
	return 0;
}
