#include <iostream>
#include <vector>
using namespace std;

/*
for(expression1; expression2; expression3)
{
    // loop body
}

expression1： 用来定义循环因子
expression2： 指明循环结束条件
expression3： 更新循环条件
*/

/*
for(declaration : expression)
{
    // loop body
}

declaration: 表示遍历声明，声明一个变量用来是存储要遍历的容器中的数据
expression: 指明要遍历的容器对象，可以是表达式、容器、数组、初始化列表
*/

int main()
{
    vector<int> vt{1, 2, 3, 4, 5, 6, 7, 8, 9};
    // 遍历
    for (auto it = vt.begin(); it != vt.end(); ++it)
    {
        cout << *it << " ";
    }

    cout << endl;

    for (auto &item : vt)
    {
        cout << item++ << " ";
    }

    cout << endl;

    for (const auto &item : vt)
    {
        cout << item << " ";
    }

    return 0;
}