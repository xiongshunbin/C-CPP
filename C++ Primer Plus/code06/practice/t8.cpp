#include<iostream>
#include<fstream>
#include<string>
#include<cstdlib>

int main()
{
	using namespace std;
	ifstream inFile;
	char ch;
	int count=0;
	string filename;
	cout<<"Enter the file name: ";
	getline(cin,filename);
	inFile.open(filename);
	if(!inFile.is_open())
	{
		cout<<"Could not open the file "<<filename<<endl;
		cout<<"Program terminating."<<endl;
		exit(EXIT_FAILURE);
	}
	inFile>>ch;
	while(inFile.good())
	{
		count++;
		inFile>>ch;
	}	
	if(inFile.eof())
		cout<<"End of file reached."<<endl;
	else
		cout<<"Input terminated for unknow reason."<<endl;
	cout<<filename<<" has "<<count<<" characters."<<endl;
	inFile.close();

	return 0;
}
