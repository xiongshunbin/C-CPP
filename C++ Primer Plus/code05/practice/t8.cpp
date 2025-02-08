#include<iostream>
#include<cstring>

const char Done[]="done";

int main()
{
	using namespace std;
	char words[20];
	int count=0;
	
	cout<<"Enter words (to stop, type the word done):\n";
	do{
		cin>>words;
		count++;
	}while(strcmp(words,Done)!=0);
	cout<<"You entered a total of "<<count-1<<" words"<<endl;
	
	return 0;
}
