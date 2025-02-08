#include<iostream>
using namespace std;

const int strsize=20;
const int usersize=5;

// Benevolent Order of Programmers name structure
void print_by_name();
void print_by_title();
void print_by_bopname();
void print_by_preference();
struct bop
{
	char fullname[strsize];		// real name
	char title[strsize];		// job title
	char bopname[strsize];		// secret BOP name
	int preference;				// 0 = fullname, 1 = title, 2 = bopname; 
	
};

bop user[usersize]=
{
	{"Wimp Macho","Software Engineer","WMSE",0},
	{"Raki Rhodes","Junior Programmer","RRJP",1},
	{"Celia Laiter","Project Manager","MIPS",2},
	{"Hoppy Hipman","Analyst Trainee","HHAT",1},
	{"Pat Hand","Project Leader ","LOOPY",2}
};

int main()
{
	cout<<"Benevolent Order of Programmers Report\n";
	cout<<"a. display by name	b. display by title\n"
		  "c. display by bopname	d. display by preference\n"
		  "q. quit\n";
	char ch;
	cout<<"Enter your choice: ";
	cin>>ch;
	while(ch!='q')
	{
		switch(ch)
		{
			case 'a':print_by_name();		break;
			case 'b':print_by_title();		break;
			case 'c':print_by_bopname();	break;
			case 'd':print_by_preference();	break;
			default:cout<<"That's not a choice.\n";
		}
		cout<<"Next choice: ";
		cin>>ch;
	}
	cout<<"Bye!\n";
	
	return 0;
}

void print_by_name()
{
	for(int i=0;i<usersize;i++)
		cout<<user[i].fullname<<endl;
}

void print_by_title()
{
	for(int i=0;i<usersize;i++)
		cout<<user[i].title<<endl;
}

void print_by_bopname()
{
	for(int i=0;i<usersize;i++)
		cout<<user[i].bopname<<endl;
}

void print_by_preference()
{
	for(int i=0;i<usersize;i++)
	{
		if(user[i].preference==0)
			cout<<user[i].fullname<<endl;
		else if(user[i].preference==1)
			cout<<user[i].title<<endl;
		else
			cout<<user[i].bopname<<endl;
	}
}
