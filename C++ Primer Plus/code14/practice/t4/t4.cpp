#include <iostream>
#include <cstring>
#include "personmi.h"
const int SIZE = 5;

using namespace std;

int main()
{
    Person* per[SIZE];
    int ct;
    for(ct = 0; ct < SIZE; ct ++)
    {
        char choice;
        cout << "Enter the category:\n"
             << "g: Gunslinger p: PokerPlayer "
             << "b: BadDude q: quit\n";
        cin >> choice;
        while(strchr("gpbq", choice) == NULL)
        {
            cout << "Please enter a g, p, b, or q: ";
            cin >> choice;
        }
        if(choice == 'q')
            break;
        switch (choice)
        {
        case 'g':
            per[ct] = new Gunslinger;
            break;
        case 'p':
            per[ct] = new PokerPlayer;
            break;
        case 'b':
            per[ct] = new BadDude;
        }
        cin.get();
        per[ct]->Set();
    }
    cout << "\nHere is: \n";
    int i;
    for(i = 0; i < ct; i ++)
    {
        cout << endl;
        per[i]->Show();
    }
    for(i = 0; i < ct; i ++)
        delete per[i];
    cout << "Bye.\n";
    return 0;
}