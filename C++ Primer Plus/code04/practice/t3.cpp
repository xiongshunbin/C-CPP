#include<iostream>
#include<cstring>
int main()
{
	using namespace std;
	const int SIZE=20;
	char firstName[SIZE];
	char lastName[SIZE];
	char fullName[2*SIZE];
	
	cout<<"Enter your first name: ";
	cin.getline(firstName,SIZE);
	cout<<"Enter your last name: ";
	cin.getline(lastName,SIZE);
	strcpy(fullName,lastName);
	strcat(fullName,", ");
	strcat(fullName,firstName);
	cout<<"Here's the information in a single string : "<<fullName<<endl;

	return 0;
}
