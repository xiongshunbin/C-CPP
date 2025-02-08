#include<iostream>
#include<cctype>

int main()
{
	using namespace std;
	char ch;

	cout<<"Please enter  characters:"<<endl;
	while((ch=cin.get())!='@')
	{
		if(islower(ch))
			ch=toupper(ch);
		else if(isupper(ch))
			ch=tolower(ch);
		else if(isdigit(ch))
			continue;
		cout<<ch;
	}
	return 0;
}
