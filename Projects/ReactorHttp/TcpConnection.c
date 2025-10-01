#include "TcpConnection.h"

#include <stdlib.h>

int processRead(void* arg)
{
	struct TcpConnection* conn = (struct TcpConnection*)arg;
	// 接收数据
	int count = bufferSocketRead(conn->readBuf, conn->channel->fd);
	if (count > 0)
	{
		// 接收到了 http 请求, 解析 http 请求
		bool flag = parseHttpRequest(conn->request, conn->readBuf, conn->response, conn->writeBuf, conn->channel->fd);
		if (!flag)
		{
			// 解析失败，回复一个简单的 html
			const char* errMsg = "HTTP/1.1 400 Bad Request\r\n\r\n";
			bufferAppendString(conn->writeBuf, errMsg);
		}
	}
	// 断开连接
	eventLoopAddTask(conn->evLoop, conn->channel, DELETE);
}

struct TcpConnection* tcpConnectionInit(int fd, struct EventLoop* evLoop)
{
	struct TcpConnection* conn = (struct TcpConnection*)malloc(sizeof(struct TcpConnection));
	conn->evLoop = evLoop;
	conn->readBuf = bufferInit(10240);
	conn->writeBuf = bufferInit(10240);
	sprintf(conn->name, "Connection-%d", fd);
	// http 协议
	conn->request = httpRequestInit();
	conn->response = httpResponseInit();
	conn->channel = channelInit(fd, ReadEvent, processRead, NULL, conn);
	eventLoopAddTask(evLoop, conn->channel, ADD);

	return conn;
}
