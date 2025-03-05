#pragma once

#include "Channel.h"
#include "EventLoop.h"

struct Dispatcher
{
	void* (*init)();														// ��ʼ��select, poll �� epoll��Ҫ�����ݿ�
	int (*add)(struct Channel* channel, struct EventLoop* evLoop);			// ���
	int (*remove)(struct Channel* channel, struct EventLoop* evLoop);		// ɾ��
	int (*modify)(struct Channel* channel, struct EventLoop* evLoop);		// �޸�
	int (*dispatch)(struct EventLoop* evLoop, int timeout);					// �¼����(timeout�ĵ�λ: s)
	int (*clear)(struct EventLoop* evLoop);									// �������(�ر��ļ������� or �ͷ��ڴ�)
};