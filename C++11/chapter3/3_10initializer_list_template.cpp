#include <iostream>
#include <vector>
using namespace std;

/**
 * 在C++的STL容器中，可以进行任意长度的数据初始化，使用初始化列表也只能进行固定参数的初始化，如果想要做到和STL一样有任意长度初始化的能力，可以使用std::inintializer_list这个轻量级的类模板来实现
 * 特点：
 *      1.轻量级的容器类型，内部定义了迭代器iterator等容器必须的概念，遍历时得到的迭代器是只读的。
 *      2.对于std::initializer_list<T>而言，可以接收任意长度的初始化列表，但要求元素必须是同种类型。
 *      3.在std::initializer_list内部有三个成员接口：size()、begin()、end()。
 *      4.std::initializer_list对象只能被整体初始化或赋值。
**/

void func(initializer_list<int> ls)
{
    auto it = ls.begin();
    for(; it != ls.end(); ++it)
    {
        cout << *it << " ";
    }
    cout << endl;
}

class Test
{
    public:
    Test(initializer_list<string> list)
    {
        for(auto it = list.begin(); it != list.end(); ++it)
        {
            cout << *it << " ";
            m_name.push_back(*it);
        }
        cout << endl;
    }
    private:
    vector<string> m_name;
};


int main()
{
    func({1, 2, 3, 4, 5});

    Test t({"jack", "lucy", "tom"});
    Test t1({"hello", "world", "nihao", "shijie"});
    return 0;
}