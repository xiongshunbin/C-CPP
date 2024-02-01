#include "personmi.h"

void Person::Data() const
{
    cout << "firstname: " << firstName;
    cout << " lsatname: " << lastName << endl;
}

void Person::Get()
{
    cout << "Enter the person's firstname: ";
    getline(cin, firstName);
    cout << "Enter the person's lastname: ";
    getline(cin, lastName);
}

Person::~Person()
{
}

void Person::Set()
{
    Get();
}

void Gunslinger::Data() const
{
    cout << "draw time: " << Draw();
    cout << " gun notches: " << notches << endl;
}

void Gunslinger::Get()
{
    cout << "Enter the gunslinger’s draw time: ";
    cin >> drawTime;
    cout << "Enter the number of notches on the gun: ";
    cin >> notches;
}

void Gunslinger::Set()
{
    Person::Get();
    Get();
}

void Gunslinger::Show() const
{
    cout << "Category: Gunslinger\n";
    Person::Data();
    Data();
}

int PokerPlayer::Draw()
{
    srand(time(nullptr));   
    return rand() % 52 + 1;
}

void PokerPlayer::Show() const
{
    cout << "Category: PokerPlayer\n";
    Person::Data();
}

double BadDude::Gdraw()
{
    return Gunslinger::Draw();
}

int BadDude::Cdraw()
{
    return PokerPlayer::Draw();
}

void BadDude::Show() const
{
    cout << "Category: BadDude\n";
    Person::Data();
    Gunslinger::Data();
}