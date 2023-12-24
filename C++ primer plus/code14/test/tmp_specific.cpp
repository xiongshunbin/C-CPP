#include <iostream>

using namespace std;

template<class T1, class T2>
class A
{
    public:
        void show();
};

template<class T1, class T2>
void A<T1, T2>::show()
{
    cout << "use general definition" << endl;
}

// explicit generate A<double, double> class
template class A<double, double>;

template <>
class A<int, int>
{
    public:
        void show();
};

void A<int, int>::show()
{
    cout << "use specialized definition" << endl;
}

template <class T1>
class A<T1, int>
{
    public:
        void show();
};

template <class T1>
void A<T1, int>::show()
{
    cout << "use partial specialized definition" << endl;
}

int main()
{
    A<char, char> a1;
    a1.show();

    A<int, int> a2;
    a2.show();

    A<char, int> a3;
    a3.show();
    return 0;
}
