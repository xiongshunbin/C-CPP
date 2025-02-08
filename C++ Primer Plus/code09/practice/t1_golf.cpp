#include <iostream>
#include <cstring>
#include "t1_golf.h"

using namespace std;

void setgolf(golf & g,const char * name,int hc)
{
    strcpy(g.fullname,name);
    g.handicap=hc;
}
int setgolf(golf & g)
{
    int temp=1;
    cout<<"The fullname is: ";
    cin.getline(g.fullname,Len);
    if(!strcmp(g.fullname,""))
        temp=0;
    else
    {
        cout<<"The handicap is: ";
        cin>>g.handicap;
        cin.get();
    }
    return temp;
}

void handicap(golf & g,int hc)
{
    g.handicap=hc;
}

void showgolf(const golf & g)
{
    cout<<g.fullname<<": "<<g.handicap<<endl;
}