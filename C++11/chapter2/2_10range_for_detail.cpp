#include <iostream>
#include <string>
#include <map>
#include <set>
#include <vector>
using namespace std;

vector<int> v{1, 2, 3, 4, 5, 6};
vector<int> &getRange()
{
    cout << "get vector range..." << endl;
    return v;
}

int main()
{
    map<int, string> m = {
        {1, "lucy"}, {2, "lily"}, {3, "tom"}};

    /**
     * 关系型容器的使用细节：
     * 1.使用普通的for循环方式（基于迭代器）遍历关系型容器，auto自动推导出的是一个迭代器类型，需要使用迭代器的方式取出元素中的键值对（和指针的操作方法相同）it->first，it->second。
     * 2.使用基于范围的for循环遍历关系型容器，auto自动推导出的类型是容器中的value_type，相当于一个队组(std::pair)对象，提取键值对的方式为：it.first，it.second。
     **/
    // 基于范围的for循环的方式
    for (auto &it : m)
    {
        cout << "id: " << it.first << ", name: " << it.second << endl;
    }

    // 普通的for循环方式
    for (auto it = m.begin(); it != m.end(); ++it)
    {
        cout << "id: " << it->first << ", name: " << it->second << endl;
    }

    /**
     * 元素只读：
     * 1. 在基于范围的for循环内部声明一个变量的引用就可以修改遍历的表达式中的元素的值，但这并不适用于所有的情况，对于set容器来说，内部元素都是只读的，这是由容器的特性决定的，因此在for循环中auto&会被视为const auto &。
     * 2. 在遍历关联容器时也会出现同样的问题，基于范围的for循环中，虽然可以得到一个std::pair引用，但是我们时不能修改里边的first函数值的，也就是key值
     **/

    set<int> st{1, 2, 3, 4, 5, 6};
    for (auto &item : st)
    {
        // cout << item++ << " ";       // error，不呢个给常量赋值
        cout << item << " ";
    }

    for (auto &item : m)
    {
        // item.first是一个常量,可以修改item.second
        // cout << "id: " << item.first++ << ", name: " << item.second << endl;
        cout << "id: " << item.first << ", name: " << item.second << endl;
    }

    /**
     * 访问的次数：
     * 基于范围的for循环遍历的对象可以是一个表达式/容器/数组/初始化列表等，假设我们对一个容器进行遍历，在遍历过程中for循环对这个容器的访问一次。在第一次访问完成时，就确定循环次数，完成后就直接进行遍历。
     * 普通的for循环方式在遍历的过程中，每次循环都需要判断边界，可以对被遍历的集合中的数据进行动态的操作。
     * 
     * 综上所述：在当需要对容器中的数据进行动态的操作时，采用普通的for循环，当不需要对容器数据进行动态的操作时使用基于范围的for循环。
     **/

    for (auto val : getRange())
    {
        cout << val << " ";
    }
    cout << endl;
    return 0;
}