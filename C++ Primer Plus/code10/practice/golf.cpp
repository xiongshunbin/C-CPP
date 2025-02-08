#include "golf.h"

Golf::Golf(const char * name,int hc)
{
    strcpy(fullname,name);
    handicap=hc;
}

Golf::Golf()
{
    int temp=1;
    cout<<"The fullname is: ";
    cin.getline(fullname,Len);
    cout<<"The handicap is: ";
    cin>>handicap;
    cin.get();
}

void Golf::sethandicap(int hc)
{
    handicap=hc;
}

void Golf::showgolf() const
{
    cout<<fullname<<": "<<handicap<<endl;
}
