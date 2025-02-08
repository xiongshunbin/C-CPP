// t2.cpp -- using exception classes
#include <iostream>
#include <cmath>    // or math.h, unix users may need -lm flag
#include <stdexcept>

// function prototypes
double hmean(double a, double b);
double gmean(double a, double b);

class bad_hmean : public std::logic_error
{
public:
    explicit bad_hmean(const std::string& what_arg = "hmean(): invalid arguments: a = -b") : logic_error(what_arg) { }
};

class bad_gmean : public std::logic_error
{
public:
    explicit bad_gmean(const std::string& what_arg = "gmean(): arguments shouble be >= 0") : logic_error(what_arg) { }
};

int main()
{
    using std::cin;
    using std::cout;
    using std::endl;

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
        catch (bad_hmean & bg)  // start of catch block
        {
            cout << bg.what() << endl;
            cout << "Try again.\n";
            continue;
        }
        catch (bad_gmean & hg)
        {
            cout << hg.what() << endl;
            cout << "Sorry, you don't get to play any more.\n";
            break;
        }   // end of catch block
    }
    cout << "Bye!\n";
    return 0;
}

double hmean(double a, double b)
{
    if(a == -b)
        throw bad_hmean();
    return 2.0 * a * b / (a + b);
}

double gmean(double a, double b)
{
    if(a < 0 || b < 0)
        throw bad_gmean();
    return std::sqrt(a * b);
}