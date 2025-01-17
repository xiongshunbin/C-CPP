#include "ThreadPool.h"

const int NUMBER = 2;

ThreadPool* threadPoolCreate(int min, int max, int qCapacity)
{
	ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));
	do
	{
		if (pool == NULL)
		{
			printf("malloc ThreadPool failed.\n");
			break;
		}
		pool->workerIDs = (pthread_t*)malloc(sizeof(pthread_t) * max);
		if (pool->workerIDs == NULL)
		{
			printf("malloc workerIDs failed.\n");
			break;
		}
		memset(pool->workerIDs, 0, sizeof(pthread_t) * max);
		pool->minNum = min;
		pool->maxNum = max;
		pool->busyNum = 0;
		pool->liveNum = min;
		pool->exitNum = 0;

		if (pthread_mutex_init(&pool->mutexPool, NULL) != 0 ||
			pthread_mutex_init(&pool->mutexBusy, NULL) != 0 ||
			pthread_cond_init(&pool->notFull, NULL) != 0 ||
			pthread_cond_init(&pool->notEmpty, NULL) != 0)
		{
			printf("init the mutex or cond failed.\n");
			break;
		}

		// ��ʼ���������
		pool->taskQueue = (Task*)malloc(sizeof(Task) * qCapacity);
		if (pool->taskQueue == NULL)
		{
			printf("malloc taskQueue failed.\n");
			break;
		}
		pool->queueCapacity = qCapacity;
		pool->queueSize = 0;
		pool->queueFront = 0;
		pool->queueRear = 0;

		pool->shutdown = 0;

		// �����߳�
		pthread_create(&pool->managerID, NULL, manager, pool);
		for (int i = 0; i < min; i++)
		{
			pthread_create(&pool->workerIDs[i], NULL, worker, pool);
		}
		return pool;
	} while (0);

	// �ͷ���Դ
	if (pool && pool->workerIDs)	free(pool->workerIDs);
	if (pool && pool->taskQueue)	free(pool->taskQueue);
	if (pool)	free(pool);

	return NULL;
}

int threadPoolDestroy(ThreadPool* pool)
{
	if (pool == NULL) return -1;
	pool->shutdown = 1;
	// �������չ������߳�
	pthread_join(pool->managerID, NULL);
	// �������й������߳�
	for (int i = 0; i < pool->liveNum; i++)
	{
		pthread_cond_signal(&pool->notEmpty);
	}
	// �ͷ���Դ
	if (pool->taskQueue) free(pool->taskQueue);
	if (pool->workerIDs) free(pool->workerIDs);

	pthread_mutex_destroy(&pool->mutexPool);
	pthread_mutex_destroy(&pool->mutexBusy);
	pthread_cond_destroy(&pool->notFull);
	pthread_cond_destroy(&pool->notEmpty);

	free(pool);
	pool = NULL;

	return 0;
}

void threadPoolAdd(ThreadPool* pool, void(*func)(void* arg), void* arg)
{
	pthread_mutex_lock(&pool->mutexPool);
	while (pool->queueSize == pool->queueCapacity && !pool->shutdown)
	{
		// �����������߳�
		pthread_cond_wait(&pool->notFull, &pool->mutexPool);
	}

	if (pool->shutdown)
	{
		pthread_mutex_unlock(&pool->mutexPool);
		return;
	}

	// �������
	pool->taskQueue[pool->queueRear].function = func;
	pool->taskQueue[pool->queueRear].arg = arg;
	pool->queueRear = (pool->queueRear + 1) % pool->queueCapacity;
	pool->queueSize++;

	// ֪ͨ�������߳�
	pthread_cond_signal(&pool->notEmpty);

	pthread_mutex_unlock(&pool->mutexPool);
}

int threadPoolBusyNum(ThreadPool* pool)
{
	pthread_mutex_lock(&pool->mutexBusy);
	int busyNum = pool->busyNum;
	pthread_mutex_unlock(&pool->mutexBusy);
	return busyNum;
}

int threadPoolALiveNum(ThreadPool* pool)
{
	pthread_mutex_lock(&pool->mutexPool);
	int aliveNum = pool->liveNum;
	pthread_mutex_unlock(&pool->mutexPool);
	return aliveNum;
}

void* worker(void* arg)
{
	ThreadPool* pool = (ThreadPool*)arg;
	while (1)
	{
		pthread_mutex_lock(&pool->mutexPool);
		// �ж���������Ƿ�Ϊ��
		while (pool->queueSize == 0 && !pool->shutdown)
		{
			// ���������߳�
			pthread_cond_wait(&pool->notEmpty, &pool->mutexPool);

			// �ж��Ƿ���Ҫ�����߳�
			if (pool->exitNum > 0)
			{
				pool->exitNum--;
				if (pool->liveNum > pool->minNum)
				{
					pool->liveNum--;
					pthread_mutex_unlock(&pool->mutexPool);
					threadExit(pool);
				}
			}
		}

		// �ж��̳߳��Ƿ�ر�
		if (pool->shutdown)
		{
			pthread_mutex_unlock(&pool->mutexPool);
			threadExit(pool);
		}

		// �����������ȡ������
		Task task;
		task.function = pool->taskQueue[pool->queueFront].function;
		task.arg = pool->taskQueue[pool->queueFront].arg;
		pool->queueFront = (pool->queueFront + 1) % pool->queueCapacity;
		pool->queueSize--;

		// ֪ͨ�������߳�
		pthread_cond_signal(&pool->notFull);

		pthread_mutex_unlock(&pool->mutexPool);

		printf("thread %ld start working......\n", pthread_self());
		pthread_mutex_lock(&pool->mutexBusy);
		pool->busyNum++;
		pthread_mutex_unlock(&pool->mutexBusy);

		task.function(task.arg);
		free(task.arg);
		task.arg = NULL;

		printf("thread %ld end working......\n", pthread_self());
		pthread_mutex_lock(&pool->mutexBusy);
		pool->busyNum--;
		pthread_mutex_unlock(&pool->mutexBusy);

	}

	return NULL;
}

void* manager(void* arg)
{
	ThreadPool* pool = (ThreadPool*)arg;
	while (!pool->shutdown)
	{
		// �������߳�ÿ��3s���һ��
		sleep(3);	
		// ��ȡ���������������������̳߳��д���̵߳�����
		pthread_mutex_lock(&pool->mutexPool);
		int queueSize = pool->queueSize;
		int liveNum = pool->liveNum;
		pthread_mutex_unlock(&pool->mutexPool);

		// ��ȡ�̳߳���æ�̵߳ĸ���
		pthread_mutex_lock(&pool->mutexBusy);
		int busyNum = pool->busyNum;
		pthread_mutex_unlock(&pool->mutexBusy);

		// ����߳�
		// ��������� > �����߳����� && �����߳����� < ����߳�����
		if (queueSize > liveNum && liveNum < pool->maxNum)
		{
			pthread_mutex_lock(&pool->mutexPool);
			int counter = 0;
			for (int i = 0; i < pool->maxNum && counter < NUMBER
				&& pool->liveNum < pool->maxNum; i++) 
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

		// �����߳�
		// æ�̵߳����� * 2 < �����߳����� && �����߳����� > ��С�߳�����
		if (busyNum * 2 < liveNum && liveNum > pool->minNum)
		{
			pthread_mutex_lock(&pool->mutexPool);
			pool->exitNum = NUMBER;
			pthread_mutex_unlock(&pool->mutexPool);
			// ֪ͨ�����߳���ɱ
			for (int i = 0; i < NUMBER; i++)
			{
				pthread_cond_signal(&pool->notEmpty);
			}
		}
	}

	return NULL;
}

void threadExit(ThreadPool* pool)
{
	pthread_t tid = pthread_self();
	for (int i = 0; i < pool->maxNum; i++)
	{
		if (pool->workerIDs[i] == tid)
		{
			pool->workerIDs[i] = 0;
			printf("threadExit() called, thread %ld exiting......\n", tid);
			break;
		}
	}
	pthread_exit(NULL);
}
