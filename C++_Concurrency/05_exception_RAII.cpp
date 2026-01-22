#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>

void function();
void funct();

class Show
{
private:
    int m_threadID;
    int m_count;
    std::string m_str;

public:
    Show(int threadID, int count, std::string str) : m_threadID(threadID), m_count(count), m_str(str) { }
    void operator()() const
    {
        for (int i = 0; i < m_count; ++i)
        {
            std::cout << "Thread " << m_threadID << ": " << m_str << std::endl;
            sleep(1);
        }
    }
};

// RAII(资源获取即初始化)
class thread_guard
{
public:
    explicit thread_guard(std::thread& thread) : m_thread(thread) { }
    ~thread_guard()
    {
        if (m_thread.joinable())
        {
            m_thread.join();
        }
    }

    thread_guard(const thread_guard&) = delete;
    thread_guard&operator=(const thread_guard&) = delete;

private:
    std::thread& m_thread;
};

int main()
{
    funct();

    for (int i = 0; i < 100; ++i)
    {
        std::cout << "Main thread is running!" << std::endl;
        sleep(1);
    }

    std::cout << "Bye!" << std::endl;

    return 0;
}

// 异常捕获
void function()
{
    int threadID = 1;
    int count = 10;
    std::string str = "Hello world!";

    Show s1(threadID, count, str);
    std::thread t1(s1);

    int n1, n2;
    std::cout << "Please enter two numbers: " << std::endl;
    std::cin >> n1 >> n2;

    try
    {
        if (n2 == 0)
        {
            throw std::runtime_error("n2 can't be 0!");
        }
        std::cout << "n1 / n2 = " << n1 / n2 << std::endl;
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "Quit with exception..." << std::endl;
        t1.join();
        return;
    }

    if (t1.joinable())
    {
        t1.join();
    }
}

// 出现异常时, 利用 RAII 等待子线程执行结束
void funct()
{
    int threadID = 1;
    int count = 10;
    std::string str = "Hello world!";

    Show s1(threadID, count, str);
    std::thread t1(s1);
    thread_guard gt(t1);

    int n1, n2;
    std::cout << "Please enter two numbers: " << std::endl;
    std::cin >> n1 >> n2;

    try
    {
        if (n2 == 0)
        {
            throw std::runtime_error("n2 can't be 0!");
        }
        std::cout << "n1 / n2 = " << n1 / n2 << std::endl;
    }
    catch(const std::runtime_error& e)
    {
        std::cout << "Quit with exception..." << std::endl;
        return;
    }
}
