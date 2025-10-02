#pragma once

#include <stdbool.h>

// ���庯��ָ��
typedef int(*handleFunc)(void* arg);

// �����ļ��������Ķ�д�¼�
enum FDEvent
{
	TimeOut = 0x01,
	ReadEvent = 0x02,
	WriteEvent = 0x04
};

struct Channel 
{
	int fd;						// �ļ�������
	int events;					// �¼�
	handleFunc readCallback;	// ���ص�
	handleFunc writeCallback;	// д�ص�
	handleFunc destroyCallback;
	void* arg;					// �ص������Ĳ���
};

// ��ʼ��Channel
struct Channel* channelInit(int fd, int events, handleFunc readFunc, handleFunc writeFunc, handleFunc destroyFunc, void* arg);

// �޸�fd��д�¼�(��� or �����)
void writeEventEnable(struct Channel* channel, bool flag);

// �ж��Ƿ����ļ���������д�¼�
bool isWriteEventEnable(struct Channel* channel);