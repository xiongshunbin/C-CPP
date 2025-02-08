#include<iostream>
#include<string>
#include<cctype>

int main()
{
	using namespace std;
	
	string word;
	int vowels,consonants,others;
	vowels=consonants=others=0;
	cout<<"Enter words (q to quit):"<<endl;
	while(cin>>word&&word!="q")
	{
		if(isalpha(word[0]))
		{
			switch(word[0])
			{
				case 'a':
				case 'A':
				case 'e':
				case 'E':
				case 'i':
				case 'I':
				case 'o':
				case 'O':
				case 'u':
				case 'U':
					vowels++;
					break;
				default:
					consonants++;
			}
		}
		else
			others++;
	}
	cout<<vowels<<" words beginning with vowels"<<endl;
	cout<<consonants<<" words beginning with consonants"<<endl;
	cout<<others<<" others"<<endl;
	
	return 0;
}
