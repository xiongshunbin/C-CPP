#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>

/**
 * ps -eLF 查看线程运行所在核心的编号
 */

void show(int threadID, int count, const std::string& str);

int main()
{
    std::cout << "Number of CPU cores: " << std::thread::hardware_concurrency() << std::endl;

    std::thread t1(show, 1, 100, "Hello world!");
    std::thread t2(show, 2, 100, "I Love You!");
    std::thread t3(show, 3, 100, "Good!");
    std::thread t4(show, 4, 100, "Hi!");

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::cout << "Bye!" << std::endl;

    return 0;
}

void show(int threadID, int count, const std::string& str)
{
    for (int i = 0; i < count; ++i)
    {
        std::cout << "Thread " << threadID << ": " << str << std::endl;
        sleep(1);
    }
}
