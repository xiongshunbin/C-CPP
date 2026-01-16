#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>

class ShowMsg
{
public:
    ShowMsg(int threadID, int count, std::string str) : m_threadID(threadID), m_count(count), m_str(str) { }

    void operator()()
    {
        for (int i = 0; i < m_count; ++i)
        {
            std::cout << "Thread " << m_threadID << ": " << m_str << std::endl;
            sleep(1);
        }
    }

private:
    int& m_threadID;
    int& m_count;
    std::string& m_str; 
};

void function();

void mainDoing()
{
    for (int i = 0; i < 20; ++i)
    {
        std::cout << "Main thread is running!" << std::endl;
        sleep(1);
    }
}

int main()
{
    function();

    std::cout << "Bye!" << std::endl;

    return 0;
}

void function()
{
    int threadID = 1;
    int count = 10;
    std::string str = "Hello world!";
    ShowMsg s1(threadID, count, str);
    std::thread t1(s1);

    t1.detach();

    mainDoing();
}
