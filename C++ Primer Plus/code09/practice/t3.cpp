#include <iostream>
#include <cstring>
#include <new>

using namespace std;

struct chaff
{
    char dross[20];
    int slag;
};

const int BUF=512;
char buffer[BUF];

void show(const chaff &);

int main()
{
    chaff *pd1=new chaff[2];
    chaff *pd2=new (buffer) chaff[2];
    char dross[20];
    int slag;
    for(int i=0;i<2;i++)
    {
        cout<<"#"<<i+1<<": "<<endl;
        cout<<"Enter the dross: ";
        cin.getline(dross,20);
        cout<<"Enter the slag: ";
        cin>>slag;
        cin.get();

        strcpy(pd1[i].dross,dross);
        strcpy(pd2[i].dross,dross);
        pd1[i].slag=pd2[i].slag=slag;
    }
    cout<<"heap: "<<pd1<<endl;
    for(int i=0;i<2;i++)
        show(pd1[i]);
    cout<<"static:"<<pd2<<endl;
    for(int i=0;i<2;i++)
        show(pd2[i]);
    delete [] pd1;
    return 0;
}

void show(const chaff & p)
{
    cout<<p.dross<<": "<<p.slag<<endl;
}
