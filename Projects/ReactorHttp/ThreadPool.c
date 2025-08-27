#include "ThreadPool.h"
#include <stdlib.h>
#include <assert.h>

struct ThreadPool* threadPoolInit(struct EventLoop* mainLoop, int count)
{
	struct ThreadPool* pool = (struct ThreadPool*)malloc(sizeof(struct ThreadPool));
	pool->mainLoop = mainLoop;
	pool->isStart = false;
	pool->threadNum = count;
	pool->workerThreads = (struct WorkerThread*)malloc(count * sizeof(struct WorkerThread));
	pool->index = 0;
	return pool;
}

void threadPoolRun(struct ThreadPool* pool)
{
	assert(pool && !pool->isStart);
	if (pool->mainLoop->threadID != pthread_self())
	{
		exit(0);
	}
	pool->isStart = true;
	if (pool->threadNum)
	{
		for (int i = 0; i < pool->threadNum; ++i)
		{
			workerThreadInit(&(pool->workerThreads[i]), i);
			workerThreadRun(&(pool->workerThreads[i]));
		}
	}
}

struct EventLoop* takeWorkerEventLoop(struct ThreadPool* pool)
{
	assert(pool->isStart);
	if (pool->mainLoop->threadID != pthread_self())
	{
		exit(0);
	}
	// 从线程池中取出一个子线程，然后取出其中的反应堆实例
	struct EventLoop* evLoop = pool->mainLoop;
	if (pool->threadNum > 0)
	{
		evLoop = pool->workerThreads[pool->index].evLoop;
		pool->index = (pool->index + 1) % pool->threadNum;
	}
	return evLoop;
}
