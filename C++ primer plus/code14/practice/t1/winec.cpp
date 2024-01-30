#include "winec.h"
#include <iostream>

using namespace std;

Wine::Wine(const char *l, int y, const int yr[], const int bot[]) : label(l), years(y), info(ArrayInt(yr, y), ArrayInt(bot, y))
{
}

Wine::Wine(const char *l, int y) : label(l), years(y), info(ArrayInt(0, y), ArrayInt(0, y))
{
}

void Wine::GetBottles()
{
    cout << "Enter " << label << " data for " << years << " year(s):" << endl;
    for(int i = 0; i < years; i ++)
    {
        int year, bottles;
        cout << "Enter year: ";
        cin >> year;
        cout << "Enter bottles for that year: ";
        cin >> bottles;
        info.first()[i] = year;
        info.second()[i] = bottles;
    }
}

void Wine::Show()
{
    cout << "Wine: " << label << endl;
    cout << "\t" << "Year" << "\t" << "Bottles" << endl;
    for(int i = 0 ; i < years; i ++)
        cout << "\t" << info.first()[i] << "\t" << info.second()[i] << endl;
}

string Wine::Label() const
{
    return label;
}

int Wine::sum()
{
    int total = info.second().sum();
    return total;
}
