#include<iostream>
#include<array>

int main()
{
	using namespace std;
	
	array<float,3> record_list;
	float average;
	
	cout<<"Enter three record of 40 meters:"<<endl;
	cout<<"First record: ";
	cin>>record_list[0];
	cout<<"Second record: ";
	cin>>record_list[1];
	cout<<"Third record: ";
	cin>>record_list[2];
	cout<<"1st: "<<record_list[0]<<"; 2nd: "<<record_list[1]<<"; 3rd: "<<record_list[2]<<endl;
	average=(record_list[0]+record_list[1]+record_list[2])/3;
	cout<<"average record: "<<average;
	
	return 0;
}
