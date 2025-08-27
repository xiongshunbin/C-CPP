#include "TcpServer.h"
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

struct TcpServer* tcpServerInit(unsigned short port, int threadNum)
{
	struct TcpServer* tcp = (struct TcpServer*)malloc(sizeof(struct TcpServer));
	tcp->threadNum = threadNum;
	tcp->mainLoop = eventLoopInit();
	tcp->threadPool = threadPoolInit(tcp->mainLoop, threadNum);
	tcp->listener = listenerInit(port);
	return tcp;
}

struct Listener* listenerInit(unsigned short port)
{
	struct Listener* listener = (struct Listener*)malloc(sizeof(struct Listener));
	// 1.�������ڼ������ļ�������
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd == -1)
	{
		perror("socket");
	}

	// 2.���ö˿ڸ���
	int opt = 1;
	int ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
	if (ret == -1)
	{
		perror("setsockopt");
	}

	// 3.��IP��ַ�Ͷ˿ں�
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	ret = bind(lfd, (struct sockaddr*)&addr, sizeof addr);
	if (ret == -1)
	{
		perror("bind");
	}

	// 4.���ü���
	ret = listen(lfd, 128);
	if (ret == -1)
	{
		perror("listen");
	}

	// 5. ����
	listener->lfd = lfd;
	listener->port = port;
	return listener;
}

int acceptConnection(void* arg)
{
	struct TcpServer* server = (struct TcpServer*)arg;
	// �Ϳͻ��˽�������
	int cfd = accept(server->listener->lfd, NULL, NULL);
	// ���̳߳���ȡ��һ�����̵߳ķ�Ӧ��ʵ������cfd
	struct EventLoop* evLoop = takeWorkerEventLoop(server->threadPool);

}

void tcpServerRun(struct TcpServer* server)
{
	// �����̳߳�
	threadPoolRun(server->threadPool);
	// ��Ӽ�������
	// ��ʼ��һ��channelʵ��
	struct Channel* channel = channelInit(server->listener->lfd, ReadEvent, acceptConnection, NULL, server);
	eventLoopAddTask(server->mainLoop, channel, ADD);
	// ������Ӧ��ģ��
	eventLoopRun(server->mainLoop);
}
