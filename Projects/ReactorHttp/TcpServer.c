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
	// 1.创建用于监听的文件描述符
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd == -1)
	{
		perror("socket");
	}

	// 2.设置端口复用
	int opt = 1;
	int ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
	if (ret == -1)
	{
		perror("setsockopt");
	}

	// 3.绑定IP地址和端口号
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	ret = bind(lfd, (struct sockaddr*)&addr, sizeof addr);
	if (ret == -1)
	{
		perror("bind");
	}

	// 4.设置监听
	ret = listen(lfd, 128);
	if (ret == -1)
	{
		perror("listen");
	}

	// 5. 返回
	listener->lfd = lfd;
	listener->port = port;
	return listener;
}

int acceptConnection(void* arg)
{
	struct TcpServer* server = (struct TcpServer*)arg;
	// 和客户端建立连接
	int cfd = accept(server->listener->lfd, NULL, NULL);
	// 从线程池中取出一个子线程的反应堆实例处理cfd
	struct EventLoop* evLoop = takeWorkerEventLoop(server->threadPool);

}

void tcpServerRun(struct TcpServer* server)
{
	// 启动线程池
	threadPoolRun(server->threadPool);
	// 添加检测的任务
	// 初始化一个channel实例
	struct Channel* channel = channelInit(server->listener->lfd, ReadEvent, acceptConnection, NULL, server);
	eventLoopAddTask(server->mainLoop, channel, ADD);
	// 启动反应堆模型
	eventLoopRun(server->mainLoop);
}
