#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include "TaskQueue.h"
#include "TaskQueue.cpp"

template <typename T>
class ThreadPool
{
public:
	ThreadPool(int min, int max);
	~ThreadPool();
	// 给线程池添加任务
	void addTask(Task<T> task);
	// 获取线程池中忙线程个数
	int getBusyNum();
	// 获取线程池中活线程个数
	int getAliveNum();

private:
	// 管理者线程任务函数
	static void* manager(void* arg);
	// 工作者线程任务函数
	static void* worker(void* arg);
	// 单个线程退出函数
	void threadExit();


private:
	// 任务队列
	TaskQueue<T>* taskQueue;

	pthread_t managerID;	// 管理者线程
	pthread_t* workerIDs;	// 工作者线程
	int minNum;				// 最小线程数
	int maxNum;				// 最大线程数
	int busyNum;			// 忙的线程数
	int liveNum;			// 活的线程数
	int exitNum;			// 销毁线程数

	pthread_mutex_t mutexPool;	// 用于保证各个线程互斥范访问线程池
	pthread_cond_t notEmpty;	// 任务队列为空时阻塞工作者线程

	static const int NUMBER = 2;

	bool shutdown = false;			// 是否销毁线程池，销毁为true，不销毁为false
};

#endif // _THREADPOOL_H_

