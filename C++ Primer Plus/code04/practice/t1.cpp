#include<iostream>
#include<string>
int main()
{
	using namespace std;
	const int size=20;
	
	char firstName[size];
	char lastName[size];
	char grade;
	int age;
	cout<<"What is your first name? ";
	cin.getline(firstName,size);
	cout<<"What is your last name? ";
	cin.getline(lastName,size);
	cout<<"What letter grade do you deserve? ";
	cin>>grade;
	cout<<"What is your age? ";
	cin>>age;
	cout<<"Name: "<<lastName<<", "<<firstName<<endl;
	cout<<"Grade: "<<char(grade+1)<<endl;
	cout<<"Age: "<<age<<endl;
	
	return 0;
}
