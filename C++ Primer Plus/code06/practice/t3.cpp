#include<iostream>
using namespace std;
bool istrue(char);

int main()
{
	cout<<"Please enter one of the following choices:"<<endl;
	cout<<"c) carnivore		p) pianist\n"
		  "t) tree			g) game\n";
	char ch;
	do
	{
		cin>>ch;
		switch (ch) 
		{
			case 'c':cout<<"A maple is a carnivore.\n";break;
			case 'p':cout<<"A maple is a pianist.\n";  break;
			case 't':cout<<"A maple is a tree.\n";     break;
			case 'g':cout<<"A maple is a game.\n";     break;
		}	
	}while(!istrue(ch));

	return 0;
}
bool istrue(char ch)
{
	if(ch=='c'||ch=='p'||ch=='t'||ch=='g')
		return true;
	else
	{
		cout<<"Please enter a c, p, t, or g: ";
		return false;
	}		
}
