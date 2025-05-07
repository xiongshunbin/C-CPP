#pragma once
#include <pthread.h>
#include "EventLoop.h"

// �������̶߳�Ӧ�Ľṹ��
struct WorkerThread
{
	pthread_t threadID;			// �߳�ID
	char name[24];				// �߳���
	pthread_mutex_t mutex;		// ������, �����߳�ͬ��
	pthread_cond_t cond;		// ��������, ���������߳�
	struct EventLoop* evLoop;	// ��Ӧ��ģ��
};

// ��ʼ���߳�
int workerThreadInit(struct WorkerThread* thread, int index);

// �����߳�
void workerThreadRun(struct WorkerThread* thread);
