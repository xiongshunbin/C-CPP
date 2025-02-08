#include <iostream>
#include <string>

using namespace std;

class Hunter
{
public:
    int m_age;              // 普通的非静态成员变量属于类对象，在没有创建类对象时并不存在
    static int m_times;     // 静态成员变量属于类，并不属于类对象，在没有创建类对象时也存在，所有的类对象共享一个静态成员变量
    string getName()
    {
        return m_name;
    }
    void setName(string name)
    {
        m_name = name;
    }

    void goHunting()
    {
        aiming();
        shoot();
    }

    // 静态成员函数也是属于类的，并不属于类对象，既可以通过类名+::作用域解析运算符来调用，也可以使用类对象+.运算符来调用
    // 静态成员函数里只能使用静态成员变量，非静态成员变量不能被直接使用。可以间接使用，方式是：将类对象作为参数传递给静态成员函数
    static void saySorry()
    {
        string count = to_string(m_times);
        cout << "Say sorry to every animal " + count + " times!" << endl;
    }

protected:
    string m_name = "Jack";
    void aiming()
    {
        cout << "使用" + m_gunName + "瞄准猎物..." << endl;
    }

private:
    string m_gunName = "AK-47";
    void shoot()
    {
        cout << "使用" + m_gunName + "射击猎物..." << endl;
    }
};
int Hunter::m_times = 3;

int main()
{
    Hunter hunter;
    cout << hunter.getName() << endl;
    return 0;
}