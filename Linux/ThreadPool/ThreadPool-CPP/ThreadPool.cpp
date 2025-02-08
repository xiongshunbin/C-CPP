#include "ThreadPool.h"

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>

template <typename T>
ThreadPool<T>::ThreadPool(int min, int max) : minNum(min), maxNum(max)
{
	do
	{
		taskQueue = new TaskQueue<T>;
		if (taskQueue == nullptr)
		{
			std::cout << "taskQueue new error." << std::endl;
			break;
		}

		workerIDs = new pthread_t[max];
		if (workerIDs == nullptr)
		{
			std::cout << "workerIDs new error." << std::endl;
			break;
		}
		memset(workerIDs, 0, sizeof(pthread_t) * max);
		busyNum = 0;
		liveNum = min;

		if (pthread_mutex_init(&mutexPool, NULL) != 0 ||
			pthread_cond_init(&notEmpty, NULL) != 0)
		{
			std::cout << "init the mutex or cond failed." << std::endl;
			break;
		}

		pthread_create(&managerID, NULL, manager, this);
		for (int i = 0; i < min; i++)
		{
			pthread_create(&workerIDs[i], NULL, worker, this);
		}
		return;
	} while (0);

	if (taskQueue) delete taskQueue;
	if (workerIDs) delete[] workerIDs;

}

template <typename T>
ThreadPool<T>::~ThreadPool()
{
	shutdown = true;

	// 阻塞回收管理者线程
	pthread_join(managerID, NULL);

	// 唤醒工作者线程
	for (int i = 0; i < liveNum; i++)
	{
		pthread_cond_signal(&notEmpty);
	}

	if (taskQueue)	delete taskQueue;
	if (workerIDs)	delete[] workerIDs;

	pthread_mutex_destroy(&mutexPool);
	pthread_cond_destroy(&notEmpty);
}

template <typename T>
void ThreadPool<T>::addTask(Task<T> task)
{
	if (shutdown)	return;

	taskQueue->addTask(task);

	pthread_cond_signal(&notEmpty);
}

template <typename T>
int ThreadPool<T>::getBusyNum()
{
	pthread_mutex_lock(&mutexPool);
	int busy_num = busyNum;
	pthread_mutex_unlock(&mutexPool);
	return busy_num;
}

template <typename T>
int ThreadPool<T>::getAliveNum()
{
	pthread_mutex_lock(&mutexPool);
	int live_num = liveNum;
	pthread_mutex_unlock(&mutexPool);
	return live_num;
}

template <typename T>
void* ThreadPool<T>::manager(void* arg)
{
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	while(!pool->shutdown)
	{
		sleep(3);

		pthread_mutex_lock(&pool->mutexPool);
		int queueSize = pool->taskQueue->taskNumber();
		int liveNum = pool->liveNum;
		int busyNum = pool->busyNum;
		pthread_mutex_unlock(&pool->mutexPool);

		// 添加线程
		// 任务个数 < 存活线程个数 && 存活线程个数 < 最大线程数
		if (queueSize > liveNum && liveNum < pool->maxNum)
		{
			pthread_mutex_lock(&pool->mutexPool);
			int counter = 0;
			for (int i = 0; i < pool->maxNum && counter < NUMBER &&
				pool->liveNum < pool->maxNum; i++)
			{
				if (pool->workerIDs[i] == 0)
				{
					pthread_create(&pool->workerIDs[i], NULL, worker, pool);
					counter++;
					pool->liveNum++;
				}
			}
			pthread_mutex_unlock(&pool->mutexPool);
		}

		// 销毁线程
		// 忙线程数 * 2 < 存活线程数 && 存活线程数 > 最大线程数
		if (busyNum * 2 < liveNum && liveNum > pool->minNum)
		{
			pthread_mutex_lock(&pool->mutexPool);
			pool->exitNum = NUMBER;
			pthread_mutex_unlock(&pool->mutexPool);

			for (int i = 0; i < NUMBER; i++)
			{
				pthread_cond_signal(&pool->notEmpty);
			}
		}
	}
	return nullptr;
}

template <typename T>
void* ThreadPool<T>::worker(void* arg)
{
	ThreadPool* pool = static_cast<ThreadPool*>(arg);

	while (true)
	{
		pthread_mutex_lock(&pool->mutexPool);
		while (pool->taskQueue->taskNumber() == 0 && !pool->shutdown)
		{
			pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

			if (pool->exitNum > 0)
			{
				pool->exitNum--;
				if (pool->liveNum > pool->minNum)
				{
					pool->liveNum--;
					pthread_mutex_unlock(&pool->mutexPool);
					pool->threadExit();
				}
			}
		}

		if (pool->shutdown)
		{
			pthread_mutex_unlock(&pool->mutexPool);
			pool->threadExit();
		}

		Task<T> task = pool->taskQueue->takeTask();

		pool->busyNum++;
		pthread_mutex_unlock(&pool->mutexPool);

		std::cout << "thread " << std::to_string(pthread_self()) << " start working..." << std::endl;

		task.function(task.arg);
		delete task.arg;
		task.arg = nullptr;

		std::cout << "thread " << std::to_string(pthread_self()) << " end working..." << std::endl;

		pthread_mutex_lock(&pool->mutexPool);
		pool->busyNum--;
		pthread_mutex_unlock(&pool->mutexPool);
	}
	return nullptr;
}

template <typename T>
void ThreadPool<T>::threadExit()
{
	pthread_t tid = pthread_self();
	for (int i = 0; i < maxNum; i++)
	{
		if (workerIDs[i] == tid)
		{
			workerIDs[i] = 0;
			std::cout << "threadExit() called, " << std::to_string(tid) << " exiting..." << std::endl;
			break;
		}
	}
	pthread_exit(NULL);
}
