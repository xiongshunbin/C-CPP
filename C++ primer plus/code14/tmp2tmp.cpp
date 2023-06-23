// tmp2tmp.cpp -- template friends to a template class
#include <iostream>
using std::cout;
using std::endl;

// template prototypes
template <typename T> void counts();
template <typename T> void report(T &);

// template class
template <class TT>
class HasFriendT
{
    private:
        TT item;
        static int ct;
    public:
        HasFriendT(const TT & i) : item(i) { ct ++; }
        ~HasFriendT() { ct --; }
        friend void counts<TT> ();
        friend void report<>(HasFriendT &);
};

template <class T>
int HasFriendT<T>::ct = 0;

// template friend functions definitions
template <class T>
void counts()
{
    cout << "template size: " << sizeof(HasFriendT<T>) << "; ";
}

template <class T>
void report(T & Temp)
{
    cout << Temp.item << endl;
}

int main()
{
    return 0;
}