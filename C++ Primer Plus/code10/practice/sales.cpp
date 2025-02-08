#include "sales.h"
using namespace std;
namespace SALES
{
    Sales::Sales(const double ar[], int n)
    {
        double total=0.0;
        int i;
        for(i=0;i<n;i++)
        {
            sales[i]=(i<n)?ar[i]:0;
            total+=sales[i];
        }
        average=total/i;
        max=sales[0];
        min=sales[0];
        for(int j=1;j<i;j++)
        {
            max=(max>sales[j])?max:sales[j];
            min=(min<sales[j])?min:sales[j];
        }
        for(int j=n;j<4;j++)
            sales[j]=0;
    }

    Sales::Sales()
    {
        double total=0.0;
        cout<<"Enter 4 sales quarters: "<<endl;
        for(int i=0;i<QUARTERS;i++)
        {
            cout<<"#"<<i+1<<": ";
            cin>>sales[i];
            total+=sales[i];  

            if(i==0)
            {
                max=sales[i];
                min=sales[i];
            } 
            else
            {
                max=(max>sales[i])?max:sales[i];
                min=(min<sales[i])?min:sales[i];
            }
        }
        average=total/QUARTERS;
    }

    void Sales::showSales() const
    {
        cout<<"Sales 4 of quarters: ";
        for(int i=0;i<QUARTERS;i++)
            cout<<sales[i]<<"  ";
        cout<<endl;
        cout<<"Max = "<<max<<endl;
        cout<<"Min = "<<min<<endl;
        cout<<"Average = "<<average<<endl;
    }
}
