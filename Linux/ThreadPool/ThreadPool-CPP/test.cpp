#include <iostream>
#include <unistd.h>
#include "ThreadPool.h"
#include "ThreadPool.cpp"

void taskFunc(void* arg)
{
	int num = *(int*)arg;
	std::cout << "thread " << std::to_string(pthread_self()) 
		<< " is working, number = " << num << std::endl;
	sleep(1);
}

int main()
{
	ThreadPool<int> pool(3, 10);
	for (int i = 0; i < 100; i++)
	{
		int* num = new int(i + 100);
		pool.addTask(Task<int>(taskFunc, num));
	}

	sleep(20);

	return 0;
}