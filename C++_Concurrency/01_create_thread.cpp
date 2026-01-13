#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>

// 通过函数指针创建线程

void showMessage();
void show(int threadNo, int count, const std::string& str);

int main()
{
    // std::thread t1(showMessage);

    std::thread t1(show, 1, 10, "Hello world!");
    std::thread t2(show, 2, 20, "I Love You!");

    t1.join();  // 阻塞主线程等待子线程执行结束
    t2.join();  // 阻塞主线程等待子线程执行结束

    std::cout << "Bye!" << std::endl;

    return 0;
}

void showMessage()
{
    for (int i = 0; i < 10; ++i)
    {
        std::cout << "Hello world!" << std::endl;
        sleep(1);
    }
}

void show(int threadNo, int count, const std::string& str)
{
    for (int i = 0; i < count; ++i)
    {
        std::cout << "Thread " << threadNo <<": " << str << std::endl;
        sleep(1);
    }
}
