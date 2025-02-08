// filefunc.cpp -- function with ostream & parameter
#include<iostream>
#include<fstream>
#include<cstdlib>

using namespace std;
const int LIMIT=5;

void file_it(ostream & os,double fo, const double fe[],int n);

int main()
{
	ofstream fout;
	const char *fn="ep-data.txt";
	
	fout.open(fn);
	if(!fout.is_open())
	{
		cout<<"Can't open "<<fn<<". Bye~~~";
		exit(EXIT_FAILURE);
	}
	double objective;
	cout<<"Enter the focal length of your "
		  "telescope objective in mm: ";
	cin>>objective;
	double eps[LIMIT];
	cout<<"Enter the focal lengths, in mm, of "<<LIMIT
		<<" eyepieces:\n";
	for(int i=0;i<LIMIT;i++)
	{
		cout<<"Eyepieces #"<<i+1<<": ";
		cin>>eps[i];
	}
	file_it(fout,objective,eps,LIMIT);
	file_it(cout,objective,eps,LIMIT);
	cout<<"Done\n";
	
	return 0;
}

void file_it(ostream & os,double fo, const double fe[],int n)
{
	os<<"Focal length of objective: "<<fo<<endl;
	os<<"f.l. eyepiece"<<" magnification"<<endl;
	for(int i=0;i<LIMIT;i++)
		os<<"\t"<<fe[i]<<"\t\t"<<int(fo/fe[i]+0.5)<<endl;
}
