// forstr2.cpp -- reversing anarray
#include<iostream>
#include<string>

int main()
{
	using namespace std;
	
	cout<<"Enter a word: ";
	string word;
	cin>>word;
	
	// physically modify string object
	char temp;
	int i,j;
	for(j=0,i=word.size()-1;j<i;j++,i--)
	{					// start block
		temp=word[i];
		word[i]=word[j];
		word[j]=temp;
	}					// end block
	cout<<word<<"\nDone\n"<<endl;
	
	return 0;
}

