#include "sales.h"
using namespace std;
namespace SALES
{
    void setSales(Sales &s, const double ar[], int n)
    {
        double total=0.0;
        int i;
        for(i=0;i<n;i++)
        {
            s.sales[i]=(i<n)?ar[i]:0;
            total+=s.sales[i];
        }
        s.average=total/i;
        s.max=s.sales[0];
        s.min=s.sales[0];
        for(int j=1;j<i;j++)
        {
            s.max=(s.max>s.sales[j])?s.max:s.sales[j];
            s.min=(s.min<s.sales[j])?s.min:s.sales[j];
        }
        for(int j=n;j<4;j++)
            s.sales[j]=0;
    }

    void setSales(Sales & s)
    {
        double total=0.0;
        cout<<"Enter 4 sales quarters: "<<endl;
        for(int i=0;i<QUARTERS;i++)
        {
            cout<<"#"<<i+1<<": ";
            cin>>s.sales[i];
            total+=s.sales[i];  

            if(i==0)
            {
                s.max=s.sales[i];
                s.min=s.sales[i];
            } 
            else
            {
                s.max=(s.max>s.sales[i])?s.max:s.sales[i];
                s.min=(s.min<s.sales[i])?s.min:s.sales[i];
            }
        }
        s.average=total/QUARTERS;
    }
    void showSales(const Sales &s)
    {
        cout<<"Sales 4 of quarters: ";
        for(int i=0;i<QUARTERS;i++)
            cout<<s.sales[i]<<"  ";
        cout<<endl;
        cout<<"Max = "<<s.max<<endl;
        cout<<"Min = "<<s.min<<endl;
        cout<<"Average = "<<s.average<<endl;
    }
}