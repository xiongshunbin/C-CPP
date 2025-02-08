#include<iostream>
#include<string>

const char Done[]="done";

int main()
{
	using namespace std;
	string words;
	int count=0;
	
	cout<<"Enter words (to stop, type the word done):\n";
	do{
		cin>>words;
		count++;
	}while(words!=Done);
	cout<<"You entered a total of "<<count-1<<" words"<<endl;
	
	return 0;
}
