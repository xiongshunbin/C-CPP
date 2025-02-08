#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>

using namespace std;

struct Patrons
{
	string name;
	double donation;
	
};
int main()
{
	int number;
	bool empty=true;
	string filename;
	ifstream inFile;
	int i=0;
	cout<<"Enter the file name: ";
	getline(cin,filename);
	
	inFile.open(filename);
	if(!inFile.is_open())
	{
		cout<<"Could not open the file."<<endl;
		cout<<"Program terminating."<<endl;
		exit(EXIT_FAILURE);
	}
	
	inFile>>number;
	if(number<=0||inFile.fail())
	{
		cout<<"Input terminated by data mismatch.\n";
		exit(EXIT_FAILURE);
	}
	Patrons* p=new Patrons[number];
	
	inFile.get();
	while(!inFile.eof()&&i<number)
	{
		cout<<"donor #"<<i+1<<":"<<endl;
		getline(inFile,p[i].name);
		cout<<"\tname:"<<p[i].name<<endl;
		inFile>>p[i].donation;
		cout<<"\tdonation: "<<p[i].donation<<endl;
		i++;
		inFile.get();
	}
	cout<<"----------Grand Patrons----------"<<endl;
	for(i=0;i<number;i++)
	{
		if(p[i].donation>10000)
		{
			cout<<"\t"<<p[i].name<<": "<<p[i].donation<<endl;
			empty=false;
		}
	}
	if(empty==true)
		cout<<"none"<<endl;
	
	cout<<"-------------Patrons-------------"<<endl;
	empty=true;
	for(i=0;i<number;i++)
	{
		if(p[i].donation<=10000)
		{
			cout<<"\t"<<p[i].name<<": "<<p[i].donation<<endl;
			empty=false;
		}
	}
	if(empty==true)
		cout<<"none";
	
	delete[] p;
	inFile.close();
	
	return 0;
}
