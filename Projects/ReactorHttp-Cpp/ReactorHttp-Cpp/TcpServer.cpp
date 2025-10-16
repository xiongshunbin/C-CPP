﻿#include "TcpServer.h"
#include "TcpConnection.h"
#include <cstdio>
#include <cstdlib>
#include <arpa/inet.h>
#include <unistd.h>
#include "spdlog/spdlog.h"

TcpServer::TcpServer(unsigned short port, int threadNum) 
	: m_port(port), m_threadNum(threadNum)
{
	m_mainLoop = new EventLoop;
	m_threadPool = new ThreadPool(m_mainLoop, threadNum);
	setListen();
}

TcpServer::~TcpServer()
{
	delete m_threadPool;
	delete m_mainLoop;
	close(m_lfd);
}

void TcpServer::setListen()
{
	// 1.创建用于监听的文件描述符
	m_lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_lfd == -1)
	{
		perror("socket");
		return;
	}

	// 2.设置端口复用
	int opt = 1;
	int ret = setsockopt(m_lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
	if (ret == -1)
	{
		perror("setsockopt");
		return;
	}

	// 3.绑定IP地址和端口号
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(m_port);
	ret = bind(m_lfd, (struct sockaddr*)&addr, sizeof addr);
	if (ret == -1)
	{
		perror("bind");
		return;
	}

	// 4.设置监听
	ret = listen(m_lfd, 128);
	if (ret == -1)
	{
		perror("listen");
		return;
	}
}

void TcpServer::run()
{
	spdlog::get("basic_logger")->info("服务器程序已经启动了......");
	spdlog::get("logger_console")->info("服务器程序已经启动了......");
	// 启动线程池
	m_threadPool->run();
	// 添加检测的任务
	// 初始化一个channel实例
	Channel* channel = new Channel(m_lfd, FDEvent::ReadEvent, acceptConnection, nullptr, nullptr, this);
	m_mainLoop->addTask(channel, ElemType::ADD);
	// 启动反应堆模型
	m_mainLoop->run();
}

int TcpServer::acceptConnection(void* arg)
{
	TcpServer* server = static_cast<TcpServer*>(arg);
	// 和客户端建立连接
	int cfd = accept(server->m_lfd, NULL, NULL);
	// 从线程池中取出一个子线程的反应堆实例处理cfd
	EventLoop* evLoop = server->m_threadPool->takeWorkerEventLoop();
	// 将 cfd 放到 TcpConnection 中处理
	new TcpConnection(cfd, evLoop);
	return 0;
}
