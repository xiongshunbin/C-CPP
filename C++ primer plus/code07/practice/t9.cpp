#include<iostream>
using namespace std;
const int SLEN=30;
struct student
{
	char fullname[SLEN];
	char hobby[SLEN];
	int ooplevel;
};
// getinfo() has two arguments: a pointer to the first element of
// an array of student structures and an int representing the
// number of elements of the array. The function solicits and
// stores data about students. It terminates input upon filling
// the array or upon encountering a blank line for the student
// name. The function returns the actual number of array elements
// filled.
int getinfo(student pa[],int n);

// dispaly1() takes a student structure as an argument
// and displays its contents
void display1(student st);

// display2() takes the adress of student structure as an
// argument and displays the structure's contents
void display2(const student *ps);

// display() takes the adress of the first element of an array
// of student structures and the number of array elements as
// arguments and displays the contents of the structures
void display3(const student pa[],int n);

int main()
{
	cout<<"Enter class size: ";
	int class_size;
	cin>>class_size;
	while(cin.get()!='\n')
		continue;
	
	student *ptr_stu=new student[class_size];
	int entered=getinfo(ptr_stu,class_size);
	for(int i=0;i<entered;i++){
		display1(ptr_stu[i]);
		display2(&ptr_stu[i]);
	}
	display3(ptr_stu,entered);
	delete [] ptr_stu;
	cout<<"Done\n";
	
	return 0;
}

int getinfo(student pa[],int n)
{
	int i;
	for(i=0;i<n;i++)
	{
		cout<<"Student #"<<i+1<<":\n";
		cout<<"\tName: ";
		cin.get(pa[i].fullname,SLEN).get();
		if(!cin)
		{
			cin.clear();
			while(cin.get()!='\n');
			break;
		}
		cout<<"\tHobby: ";
		cin.getline(pa[i].hobby,SLEN);
		if(!cin)
		{
			cin.clear();
			while(cin.get()!='\n');
			break;
		}
		cout<<"\tOoplevel: ";
		cin>>pa[i].ooplevel;
		if(!cin)
		{
			cin.clear();
			while(cin.get()!='\n');
			break;
		}
		cin.get();
	}
	return i;
}

void display1(student st)
{
	cout<<"Fillname: "<<st.fullname<<endl;
	if(st.hobby)
		cout<<"Hobby: no"<<endl;
	else
		cout<<"Hobby: "<<st.hobby<<endl;
	cout<<"Ooplevel: "<<st.ooplevel<<endl;
}

void display2(const student *ps)
{
	cout<<"Fillname: "<<ps->fullname<<endl;
	if(ps->hobby)
		cout<<"Hobby: no"<<endl;
	else
		cout<<"Hobby: "<<ps->hobby<<endl;
	cout<<"Ooplevel: "<<ps->ooplevel<<endl;
}
void display3(const student pa[],int n)
{
	for(int i=0;i<n;i++)
	{
		cout<<"Fillname: "<<pa[i].fullname<<endl;
		if(pa[i].hobby)
			cout<<"Hobby: no"<<endl;
		else
			cout<<"Hobby: "<<pa[i].hobby<<endl;
		cout<<"Ooplevel: "<<pa[i].ooplevel<<endl;
	}
}
