#include "classic2.h"
#include <cstring>
#include <iostream>

using std::cout;
using std::endl;

Cd::Cd(const char * s1, const char * s2, int n, double x)
{
    performers = new char[strlen(s1) + 1];
    strcpy(performers, s1);
    label = new char[strlen(s2) + 1];
    strcpy(label, s2);

    selections = n;
    playtime = x;
}

Cd::Cd(const Cd & d)
{
    performers = new char[strlen(d.performers) + 1];
    label = new char[strlen(d.label) + 1];
    strcpy(performers, d.performers);
    strcpy(label, d.label);
    selections = d.selections;
    playtime = d.playtime;
}

Cd::Cd()
{
    performers = NULL;
    label = NULL;
    selections = 0;
    playtime = 0;
}
Cd::~Cd()
{
    delete [] performers;
    delete [] label;
}

void Cd::Report() const
{
    cout << "Performers: " << performers << endl;
    cout << "Label: " << label << endl;
    cout << "Selections: "<< selections << endl;
    cout << "Playtime: " << playtime << endl;
}

Cd & Cd::operator=(const Cd & d)
{
    if(this == &d)
        return *this;
    delete [] performers;
    delete [] label;
    performers = new char[strlen(d.performers) + 1];
    label = new char[strlen(d.label) + 1];
    strcpy(performers, d.performers);
    strcpy(label, d.label);
    selections = d.selections;
    playtime = d.playtime;
    return *this;
}

Classic::Classic() : Cd()
{
    works = NULL;
}

Classic::Classic(const Classic & c) : Cd(c)
{
    works = new char[strlen(c.works) + 1];
    strcpy(works, c.works);
}

Classic::Classic(const char * s1, const char * s2, const char * s3, int n, double x) : Cd(s2, s3, n, x)
{
    works = new char[strlen(s1) + 1];
    strcpy(works, s1);
}

Classic::~Classic()
{
    delete [] works;
}

void Classic::Report() const
{
    Cd::Report();
    cout << "Works: " << works << endl;
}

Classic & Classic::operator=(const Classic & c)
{
    if(this == &c)
        return *this;
    Cd::operator=(c);
    delete [] works;
    works = new char[strlen(c.works) + 1];
    strcpy(works, c.works);
    return *this;
}