#include <iostream>
#include "t1_golf.h"
using namespace std;
const int Size=3;
int main()
{
    golf g[Size];
    int count = 0;
    cout << "Please enter the information of golf players: " << endl;
    while(count<Size&&setgolf(g[count]))
    {
        count++;
        if(count<Size)
            cout<<"Please enter next player: "<<endl;
    }

    cout<<"\nShow all golf players informations: "<<endl;
    for(int i=0;i<count;i++)
        showgolf(g[i]);
    
    cout<<"\nReset all the players information: "<<endl;
    for(int i=0;i<count;i++)
    {
        handicap(g[i],90);
        showgolf(g[i]);
    }
    return 0;
}