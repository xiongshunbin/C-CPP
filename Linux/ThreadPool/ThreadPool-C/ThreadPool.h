#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


// ����ṹ��
typedef struct Task
{
	void (*function)(void* arg);
	void* arg;
} Task;

// �̳߳ؽṹ��
typedef struct ThreadPool
{
	// �������
	Task* taskQueue;	
	int queueCapacity;	// ����
	int queueSize;		// ��ǰ�������
	int queueFront;		// ��ͷ	-> ȡ����
	int queueRear;		// ��β	-> ������

	pthread_t managerID;	// �������߳�ID

	pthread_t* workerIDs;	// �������߳�ID
	int minNum;				// ��С�߳���
	int maxNum;				// ����߳���
	int busyNum;			// æ�̸߳���
	int liveNum;			// ����߳���
	int exitNum;			// �˳��߳���

	pthread_mutex_t mutexPool;	// ���ڱ�֤�����̻߳�������̳߳�
	pthread_mutex_t mutexBusy;	// ���ڱ�������busyNum����ʱ�Ļ���
	pthread_cond_t notFull;		// ���������ʱ������
	pthread_cond_t notEmpty;	// ������п�ʱ������

	int shutdown;			// �̳߳��Ƿ�����, ����Ϊ1, ������Ϊ0
} ThreadPool;

// �����̳߳ز���ʼ��
ThreadPool* threadPoolCreate(int min, int max, int qCapacity);

// �����̳߳�
int threadPoolDestroy(ThreadPool* pool);

// ���̳߳��������
void threadPoolAdd(ThreadPool* pool, void (*func)(void* arg), void* arg);

// ��ȡ�̳߳��й������̸߳���
int threadPoolBusyNum(ThreadPool* pool);

// ��ȡ�̳߳��л��ŵ��̸߳���
int threadPoolALiveNum(ThreadPool* pool);

void* worker(void* arg);
void* manager(void* arg);
void threadExit(ThreadPool* pool);

#endif	// _THREADPOOL_H_