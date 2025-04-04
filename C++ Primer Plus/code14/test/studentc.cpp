// studentc.cpp -- Student class using containment
#include "studentc.h"
using std::ostream;
using std::endl;
using std::istream;
using std::string;

// public methods

Student::Student(){
    name = "Null Student";
    scores = ArrayDb();
}
Student::Student(const std::string & s) {
    name = s;
    scores = ArrayDb();
}
Student::Student(int n) {
    name = "Nully";
    scores = ArrayDb(n);
}
Student::Student(const std::string & s, int n){
    name = s;
    scores = ArrayDb(n);
}
Student::Student(const std::string & s, const ArrayDb & a) {
    name = s;
    scores = a;
}
Student::Student(const char *str, const double * pd, int n){
    name = str;
    scores = ArrayDb(pd, n);
}

double Student::Average() const
{
    if(scores.size() > 0)
        return scores.sum() / scores.size();
    else
        return 0;
}

const string & Student::Name() const
{
    return name;
}

double & Student::operator[](int i)
{
    return scores[i]; // use valarray<double>::operator[]()
}

double Student::operator[](int i) const
{
    return scores[i];
}

// private method
ostream & Student::arr_out(ostream & os) const
{
    int i;
    int lim = scores.size();
    if(lim > 0)
    {
        for(int i = 0; i < lim; i ++)
        {
            os << scores[i] << " ";
            if(i % 5 == 4)
                os << endl;
        }
        if(i % 5 == 4)
            os << endl;
    }
    else
        os << " empty array ";
    return os;
}

// friends
// use string version of operator>>()
istream & operator>>(istream & is, Student & stu)
{
    is >> stu.name;
    return is;
}

istream & getline(istream & is, Student & stu)
{
    getline(is, stu.name);
    return is;
}

// use string version of operator<<()
ostream & operator<<(ostream & os, const Student & stu)
{
    os << "Scores for " << stu.name << ":\n";
    stu.arr_out(os);
    return os;
}