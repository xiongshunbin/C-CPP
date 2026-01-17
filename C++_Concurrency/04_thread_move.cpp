#include <iostream>
#include <thread>
#include <string>
#include <unistd.h>

/**
 * move(): 线程移动, 将一个线程移动到另外一个线程, 则原来的线程就被销毁掉
 * joinable(): 判断当前线程是否能够汇合, 能汇合返回 true, 否则返回 false
 * 
 * joinable() 返回 false 的情况:
 *      1. 当线程执行过 move() 后, 将不能汇合
 *      2. 已执行过 join() 或 detach() 后, 都将不能汇合
 *      3. 如果线程是被默认构造的线程
 * 
 * get_id() 获取线程 ID
 * 
 * 当子线程调用 detach() 后, 如果主线程结束并退出会导致子线程也退出
 */

void show(int count, const std::string& str);

int main()
{
    std::thread t1(show, 10, "Hello world!");
    std::cout << "Thread1 id = " << t1.get_id() << std::endl;
    sleep(5);

    std::thread t2 = std::move(t1);
    std::cout << "Threa2 id = " << t2.get_id() << std::endl;

    std::cout << "t1.joinable = " << t1.joinable() << std::endl;
    std::cout << "t2.joinable = " << t2.joinable() << std::endl;

    if (t1.joinable())
    {
        t1.join();
    }

    if (t2.joinable())
    {
        t2.detach();
    }

    std::cout << "t2.joinable = " << t2.joinable() << std::endl;

    std::thread t3;
    std::cout << "t3.joinable = " << t3.joinable() << std::endl;
    if (t3.joinable())
    {
        t3.join();
    }

    std::cout << "Bye!" << std::endl;

    return 0;
}

void show(int count, const std::string& str)
{
    for (int i = 0; i < count; ++i)
    {
        std::cout << str << std::endl;
        sleep(1);
    }
}
