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
	// ���̳߳��������
	void addTask(Task<T> task);
	// ��ȡ�̳߳���æ�̸߳���
	int getBusyNum();
	// ��ȡ�̳߳��л��̸߳���
	int getAliveNum();

private:
	// �������߳�������
	static void* manager(void* arg);
	// �������߳�������
	static void* worker(void* arg);
	// �����߳��˳�����
	void threadExit();


private:
	// �������
	TaskQueue<T>* taskQueue;

	pthread_t managerID;	// �������߳�
	pthread_t* workerIDs;	// �������߳�
	int minNum;				// ��С�߳���
	int maxNum;				// ����߳���
	int busyNum;			// æ���߳���
	int liveNum;			// ����߳���
	int exitNum;			// �����߳���

	pthread_mutex_t mutexPool;	// ���ڱ�֤�����̻߳��ⷶ�����̳߳�
	pthread_cond_t notEmpty;	// �������Ϊ��ʱ�����������߳�

	static const int NUMBER = 2;

	bool shutdown = false;			// �Ƿ������̳߳أ�����Ϊtrue��������Ϊfalse
};

#endif // _THREADPOOL_H_

