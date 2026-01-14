#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>

// #define FUNCTION_POINTER
// #define FUNCTION_OBJECT
// #define LAMBDA
#define OBJECT_MEMBER

/**
 * C++11创建线程: 
 *      1.通过函数指针创建线程
 *      2.通过函数对象(函数符、仿函数)创建线程
 *      3.通过Lambda表达式创建线程
 *      4.通过成员函数创建线程
 */

 // 通过函数指针创建线程
void showMessage();
void show(int threadNo, int count, const std::string& str);

// 通过函数对象创建线程
class ShowMsg
{
public:
    ShowMsg() { }
    ShowMsg(int threadNo, int count, std::string str) : 
    m_threadNo(threadNo), m_count(count), m_str(str) { }

    void operator()() const
    {
        for (int i = 0; i < m_count; ++i)
        {
            std::cout << "Thread " << m_threadNo <<": " << m_str << std::endl;
            sleep(1);
        }
    }

    void dispaly(int threadNo, int count, const std::string& str)
    {
        for (int i = 0; i < count; ++i)
        {
            std::cout << "Thread " << threadNo <<": " << str << std::endl;
            sleep(1);
        }
    }

private:
    int m_threadNo;
    int m_count;
    std::string m_str;
};

int main()
{
#ifdef FUNCTION_POINTER

    // std::thread t1(showMessage);

    std::thread t1(show, 1, 10, "Hello world!");
    std::thread t2(show, 2, 20, "I Love You!");

#elifdef FUNCTION_OBJECT

    // ShowMsg s1(1, 10, "Hello world!");
    ShowMsg s2(2, 20, "I Love You!");

    std::thread t1(ShowMsg(1, 10, "Hello world!"));
    std::thread t2(s2);

#elifdef LAMBDA

    std::thread t3([](int threadNo, int count, const std::string& str){
        for (int i = 0; i < count; ++i)
        {
            std::cout << "Thread " << threadNo <<": " << str << std::endl;
            sleep(1);
        }
    }, 3, 30, "Good!");

#elifdef OBJECT_MEMBER

    ShowMsg s4;
    std::thread t4(&ShowMsg::dispaly, s4, 4, 40, "He! Hi! HaHaHa!");

#endif

    // t1.join();  // 阻塞主线程等待子线程执行结束
    // t2.join();  // 阻塞主线程等待子线程执行结束
    // t3.join();
    t4.join();

    std::cout << "Bye!" << std::endl;

    return 0;
}

// 不带参数的线程函数
void showMessage()
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Hello world!" << std::endl;
        sleep(1);
    }
}

// 带参数的线程函数
void show(int threadNo, int count, const std::string& str)
{
    for (int i = 0; i < count; ++i)
    {
        std::cout << "Thread " << threadNo <<": " << str << std::endl;
        sleep(1);
    }
}
