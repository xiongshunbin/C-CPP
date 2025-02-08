// t3.cpp -- using exception classes
#include <iostream>
#include <cmath>    // or math.h, unix users may need -lm flag
#include <stdexcept>

using std::cin;
using std::cout;
using std::endl;

// function prototypes
double hmean(double a, double b);
double gmean(double a, double b);

class bad_mean : public std::logic_error
{
private:
    double v1;
    double v2;
public:
    bad_mean(double a = 0, double b = 0, const std::string & what_arg = "mean error") 
        : logic_error(what_arg), v1(a), v2(b) { }
    virtual ~bad_mean() { }
    virtual void show() const;
};

class bad_hmean : public bad_mean
{
public:
    explicit bad_hmean(double a, double b, const std::string& what_arg =
        "error: hmean() invalid arguments: a = -b") : bad_mean(a, b, what_arg) { }
    virtual void show() const;
};

class bad_gmean : public bad_mean
{
public:
    explicit bad_gmean(double a = 0, double b = 0, const std::string& what_arg = 
        "error: gmean() arguments shouble be >= 0") : bad_mean(a, b, what_arg) { }
    virtual void show() const;
};

void bad_mean::show() const
{
    cout << "Values used: " << v1 << ", " << v2 << endl;
}

void bad_hmean::show() const
{
    cout << what() << endl;
    bad_mean::show();
}

void bad_gmean::show() const
{
    cout << what() << endl;
    bad_mean::show();
}

int main()
{
    double x, y, z;

    cout << "Enter two numbers: ";
    while(cin >> x >> y)
    {
        try {       // start of try block
            z = hmean(x, y);
            cout << "Harmonic mean of " << x << " and " << y
                << " is " << z << endl;
            z = gmean(x, y);
            cout << "Geometric mean of " << x << " and " << y
                << " is " << z << endl;
            cout << "Enter next set of numbers <q to quit>: ";
        }           // end of try block
        catch (bad_mean & bg)  // start of catch block
        {
            bg.show();
            break;
        }   // end of catch block
    }
    cout << "Bye!\n";
    return 0;
}

double hmean(double a, double b)
{
    if(a == -b)
        throw bad_hmean(a, b);
    return 2.0 * a * b / (a + b);
}

double gmean(double a, double b)
{
    if(a < 0 || b < 0)
        throw bad_gmean(a, b);
    return std::sqrt(a * b);
}
