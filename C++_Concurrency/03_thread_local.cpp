#include <iostream>
#include <thread>
#include <unistd.h>

/**
 * thread_local: 线程的本地存储
 */

int g_i = 0;
thread_local int g_j = 0;

void threadFunction(int threadID);

int main()
{
    std::thread t1(threadFunction, 1);
    t1.join();

    std::thread t2(threadFunction, 2);
    t2.join();

    std::cout << "Main thread: i = " << g_i << ", j = " << g_j << std::endl;

    return 0;
}

void threadFunction(int threadID)
{
    std::cout << "Thread " << threadID << ": i = " << g_i << ", j = " << g_j << std::endl;

    ++g_i;
    ++g_j;
}
