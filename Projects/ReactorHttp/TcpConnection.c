#include "TcpConnection.h"
#include <stdlib.h>
#include <stdio.h>
#include "Log.h"

int processRead(void* arg)
{
	struct TcpConnection* conn = (struct TcpConnection*)arg;
	// 接收数据
	int count = bufferSocketRead(conn->readBuf, conn->channel->fd);
	Debug("服务器接收到的http请求数据: %s", conn->readBuf->data + conn->readBuf->readPos);

	if (count > 0)
	{
		// 接收到了 http 请求, 解析 http 请求
#ifdef MSG_SEND_AUTO
		writeEventEnable(conn->channel, true);
		eventLoopAddTask(conn->evLoop, conn->channel, MODIFY);
#endif
		bool flag = parseHttpRequest(conn->request, conn->readBuf, conn->response, conn->writeBuf, conn->channel->fd);
		if (!flag)
		{
			// 解析失败，回复一个简单的 html
			const char* errMsg = "HTTP/1.1 400 Bad Request\r\n\r\n";
			bufferAppendString(conn->writeBuf, errMsg);
#ifndef MSG_SEND_AUTO
			bufferSendData(conn->writeBuf, conn->channel->fd);
#endif
		}
	}
	else
	{
#ifdef MSG_SEND_AUTO
		// 断开连接
		eventLoopAddTask(conn->evLoop, conn->channel, DELETE);
#endif
	}
#ifndef MSG_SEND_AUTO
	// 断开连接
	eventLoopAddTask(conn->evLoop, conn->channel, DELETE);
#endif
	return 0;
}

int processWrite(void* arg)
{
	Debug("开始发送数据(基于写事件发送)......");
	struct TcpConnection* conn = (struct TcpConnection*)arg;
	// 发送数据
	int count = bufferSendData(conn->writeBuf, conn->channel->fd);
	if (count > 0)
	{
		// 判断数据是否被全部发送出去了
		if (bufferReadableSize(conn->writeBuf) == 0)
		{
			// 1.不再检测写事件 -- 修改 Channel 中保存的事件
			writeEventEnable(conn->channel, false);
			// 2.修改 Dispatcher 检查的集合 -- 添加任务节点
			eventLoopAddTask(conn->evLoop, conn->channel, MODIFY);
			// 3.删除节点
			eventLoopAddTask(conn->evLoop, conn->channel, DELETE);
		}
	}
	return 0;
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
	conn->channel = channelInit(fd, ReadEvent, processRead, processWrite, tcpConnectionDestroy, conn);
	eventLoopAddTask(evLoop, conn->channel, ADD);
	Debug("服务器和客户端建立连接, ThreadName: %s, ThreadID: %lu, ConnectionName: %s",
		evLoop->threadName, evLoop->threadID, conn->name);

	return conn;
}

int tcpConnectionDestroy(void* arg)
{
	struct TcpConnection* conn = (struct TcpConnection*)arg;
	if (conn != NULL)
	{
		Debug("连接断开, 释放资源, ConnectionName: %s", conn->name);

		if (conn->readBuf != NULL && bufferReadableSize(conn->readBuf) == 0
			&& conn->writeBuf != NULL && bufferReadableSize(conn->writeBuf) == 0)
		{
			destroyChannel(conn->evLoop, conn->channel);
			bufferDestroy(conn->readBuf);
			bufferDestroy(conn->writeBuf);
			httpRequestDestroy(conn->request);
			httpResponseDestroy(conn->response);
			free(conn);
		}
	}
	return 0;
}
