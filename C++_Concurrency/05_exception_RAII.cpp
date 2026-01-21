#include <iostream>
#include <thread>
#include <unistd.h>
#include <string>

void function();

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

int main()
{
    function();

    for (int i = 0; i < 100; ++i)
    {
        std::cout << "Main thread is running!" << std::endl;
        sleep(1);
    }

    std::cout << "Bye!" << std::endl;

    return 0;
}

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

    t1.join();
}
