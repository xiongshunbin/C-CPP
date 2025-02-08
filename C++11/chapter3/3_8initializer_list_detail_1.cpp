#include <iostream>
using namespace std;

/**
 * 使用初始化列表初始化聚合体类型：
 *      1.普通数组本身可以看作是一个聚合类型。
 *      2.满足一下条件的类（class、struct、union）可以看作是一个聚合类型：
 *          a.无用户自定义的构造函数
 *          b.无私有或保护的非静态数据成员
 *      3.无基类
 *      4.无虚函数
 **/

/*a.类中有私有或保护数据成员, 无法使用列表初始化进行初始化
struct T1
{
    int x;
    long y;

protected:
    int z;
} t{1, 100, 2};// 在类的外部是无法访问类的私有或保护数据成员的，故被称为非聚合类型
*/

/* b.类中有非静态成员可以通过列表初始化进行初始化，但它不能初始化静态成员变量。)*/
struct T2
{
    int x;
    long y;

protected:
    static int z;
} t{1, 100};
int T2::z = 2;  // 静态成员的初始化

struct T
{
    int a = 5;
    int array[3]{1, 2, 3};
    int c;
};

int main(void)
{
    T t{11, {2, 3, 4}, 12};
    cout << t.a << ", " << t.c << endl;
    for(int i = 0; i < 3; i++)
    {
        cout << t.array[i] << " ";
    }
    cout << endl;
    return 0;
}