#ifndef _THREADPOOL_H_
#define _THREADPOOL_H_

#include<stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>


// 任务结构体
typedef struct Task
{
	void (*function)(void* arg);
	void* arg;
} Task;

// 线程池结构体
typedef struct ThreadPool
{
	// 任务队列
	Task* taskQueue;	
	int queueCapacity;	// 容量
	int queueSize;		// 当前任务个数
	int queueFront;		// 队头	-> 取数据
	int queueRear;		// 队尾	-> 放数据

	pthread_t managerID;	// 管理者线程ID

	pthread_t* workerIDs;	// 工作者线程ID
	int minNum;				// 最小线程数
	int maxNum;				// 最大线程数
	int busyNum;			// 忙线程个数
	int liveNum;			// 存活线程数
	int exitNum;			// 退出线程数

	pthread_mutex_t mutexPool;	// 用于保证各个线程互斥访问线程池
	pthread_mutex_t mutexBusy;	// 用于保护更新busyNum变量时的互斥
	pthread_cond_t notFull;		// 任务队列满时的阻塞
	pthread_cond_t notEmpty;	// 任务队列空时的阻塞

	int shutdown;			// 线程池是否销毁, 销毁为1, 不销毁为0
} ThreadPool;

// 创建线程池并初始化
ThreadPool* threadPoolCreate(int min, int max, int qCapacity);

// 销毁线程池
int threadPoolDestroy(ThreadPool* pool);

// 给线程池添加任务
void threadPoolAdd(ThreadPool* pool, void (*func)(void* arg), void* arg);

// 获取线程池中工作的线程个数
int threadPoolBusyNum(ThreadPool* pool);

// 获取线程池中活着的线程个数
int threadPoolALiveNum(ThreadPool* pool);

void* worker(void* arg);
void* manager(void* arg);
void threadExit(ThreadPool* pool);

#endif	// _THREADPOOL_H_