#ifndef __PERSONMI_H__
#define __PERSONMI_H__

#include <iostream>
#include <string>

using namespace std;

class Person
{
private:
    string firstName;
    string lastName;
protected:
    virtual void Data() const;
    virtual void Get();
public:
    Person() : firstName("no"), lastName("one") { }
    Person(const string& fname, const string& lname) : firstName(fname), lastName(lname) { }
    virtual ~Person();
    virtual void Set();
    virtual void Show() const = 0;
};

class Gunslinger : virtual public Person
{
private:
    double drawTime;
    int notches;
protected:
    void Data() const;
    void Get();
public:
    Gunslinger() : Person(), drawTime(0), notches(0) { }
    Gunslinger(const string& fname, const string& lname, double dtime, int n) : Person(fname, lname), drawTime(dtime), notches(n) { }
    Gunslinger(const Person& p, double dtime, int n) : Person(p), drawTime(dtime), notches(n) { }
    void Set();
    void Show() const;
    double Draw() const { return drawTime; }
};

class PokerPlayer : virtual public Person
{
public:
    PokerPlayer() : Person() { }
    PokerPlayer(const string& fname, const string& lname) : Person(fname, lname) { }
    PokerPlayer(const Person& p) : Person(p) { }
    int Draw();
    void Show() const;
};

class BadDude : public Gunslinger, public PokerPlayer
{
public:
    BadDude() { }
    BadDude(const string& fname, const string& lname, double dtime, int n) 
        : Person(fname, lname), Gunslinger(fname, lname, dtime, n), PokerPlayer(fname, lname) { }
    BadDude(const Person& p, double dtime, int n) : Person(p), Gunslinger(p, dtime, n), PokerPlayer(p) { }
    BadDude(const Gunslinger& g) : Person(g), Gunslinger(g), PokerPlayer(g) { }
    BadDude(const PokerPlayer& p, double dtime, int n) : Person(p), Gunslinger(p, dtime, n), PokerPlayer(p) { }
    double Gdraw();
    int Cdraw();
    void Show() const;
};

#endif