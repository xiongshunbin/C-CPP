#include "winec.h"
#include <iostream>

using namespace std;

Wine::Wine(const char *l, int y, const int yr[], const int bot[]) : string(l), years(y), PairArray(ArrayInt(yr, y), ArrayInt(bot, y))
{
}

Wine::Wine(const char *l, int y) : string(l), years(y), PairArray(ArrayInt(y), ArrayInt(y))
{
}

void Wine::GetBottles()
{
    cout << "Enter " << (const string &)*this << " data for " << years << " year(s):" << endl;
    for(int i = 0; i < years; i ++)
    {
        int year, bottles;
        cout << "Enter year: ";
        cin >> year;
        cout << "Enter bottles for that year: ";
        cin >> bottles;
        PairArray::first()[i] = year;
        PairArray::second()[i] = bottles;
    }
}

void Wine::Show()
{
    cout << "Wine: " << (const string &)*this << endl;
    cout << "\t" << "Year" << "\t" << "Bottles" << endl;
    for(int i = 0 ; i < years; i ++)
        cout << "\t" << PairArray::first()[i] << "\t" << PairArray::second()[i] << endl;
}

string Wine::Label() const
{
    return (const string &)*this;
}

int Wine::sum()
{
    int total = PairArray::second().sum();
    return total;
}
