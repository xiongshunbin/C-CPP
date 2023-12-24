#include <iostream>

using namespace std;

template<class T1, class T2, class T3>
class Trio
{
    public:
        void show();
};

template<class T1, class T2, class T3>
void Trio<T1, T2, T3>::show()
{
    cout << "use general template" << endl;
}

template<class T1, class T2>
class Trio<T1, T2, T2>
{
    public:
        void show();
};

template<class T1, class T2>
void Trio<T1, T2, T2>::show()
{
    cout << "specialization with T3 set to T2" << endl;
}

template<class T1>
class Trio<T1, T1 *, T1 *>
{
    public:
        void show();
};

template<class T1>
void Trio<T1, T1 *, T1 *>::show()
{
    cout << "specialization with T3 and T2 set to T1 *" << endl;
}

int main()
{
    Trio<int, short, char *> t1;
    t1.show();

    Trio<int, short, short> t2;
    t2.show();

    Trio<char, char *, char *> t3;
    t3.show();
    return 0;
}