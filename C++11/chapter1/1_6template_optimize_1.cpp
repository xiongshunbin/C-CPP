#include <iostream>
#include <string>
#include <map>
using namespace std;

template <typename T>
class Container
{
public:
    void print(T &t)
    {
        auto it = t.begin();
        for (; it != t.end(); ++it)
        {
            cout << it->first << ", " << it->second << endl;
        }
    }
};

#if 0
template<typename T = long, typename U = int>
void mytest(T t = 'A', U u = 'B')
{
    cout << "t: " << t << ", u: " << u << endl;
}
#endif

int main(int argc, char *argv[])
{
#if 0
    mytest('a', 'b');
    mytest<int>('a', 'b');
    mytest<char>('a', 'b');
    mytest<int, char>('a', 'b');
    mytest<char, int>('a', 'b');
    mytest();
#endif

    map<int, string> m;
    m.insert(make_pair(1, "ace"));
    m.insert(make_pair(2, "sabo"));
    m.insert(make_pair(3, "luffy"));
    Container<map<int, string>> c;
    c.print(m);

    return 0;
}
