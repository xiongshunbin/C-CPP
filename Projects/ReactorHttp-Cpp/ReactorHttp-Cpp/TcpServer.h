#pragma once

#include "EventLoop.h"
#include "ThreadPool.h"

class TcpServer
{
public:
	TcpServer(unsigned short port, int threadNum);
	~TcpServer();

	// 初始化用于监听的文件描述符
	void setListen();

	// 启动服务器
	void run();

private:
	static int acceptConnection(void* arg);

private:
	unsigned short m_port;
	int m_threadNum;
	EventLoop* m_mainLoop = nullptr;
	ThreadPool* m_threadPool = nullptr;
	int m_lfd;
};
