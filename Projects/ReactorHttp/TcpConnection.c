#include "TcpConnection.h"
#include <stdlib.h>
#include <stdio.h>

int processRead(void* arg)
{
	struct TcpConnection* conn = (struct TcpConnection*)arg;
	// ��������
	int count = bufferSocketRead(conn->readBuf, conn->channel->fd);
	if (count > 0)
	{
		// ���յ��� http ����, ���� http ����
#ifdef MSG_SEND_AUTO
		writeEventEnable(conn->channel, true);
		eventLoopAddTask(conn->evLoop, conn->channel, MODIFY);
#endif
		bool flag = parseHttpRequest(conn->request, conn->readBuf, conn->response, conn->writeBuf, conn->channel->fd);
		if (!flag)
		{
			// ����ʧ�ܣ��ظ�һ���򵥵� html
			const char* errMsg = "HTTP/1.1 400 Bad Request\r\n\r\n";
			bufferAppendString(conn->writeBuf, errMsg);
		}
	}
#ifndef MSG_SEND_AUTO
	// �Ͽ�����
	eventLoopAddTask(conn->evLoop, conn->channel, DELETE);
#endif
	return 0;
}

int processWrite(void* arg)
{
	struct TcpConnection* conn = (struct TcpConnection*)arg;
	// ��������
	int count = bufferSendData(conn->writeBuf, conn->channel->fd);
	if (count > 0)
	{
		// �ж������Ƿ�ȫ�����ͳ�ȥ��
		if (bufferReadableSize(conn->writeBuf) == 0)
		{
			// 1.���ټ��д�¼� -- �޸� Channel �б�����¼�
			writeEventEnable(conn->channel, false);
			// 2.�޸� Dispatcher ���ļ��� -- �������ڵ�
			eventLoopAddTask(conn->evLoop, conn->channel, MODIFY);
			// 3.ɾ���ڵ�
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
	// http Э��
	conn->request = httpRequestInit();
	conn->response = httpResponseInit();
	conn->channel = channelInit(fd, ReadEvent, processRead, processWrite, tcpConnectionDestroy, conn);
	eventLoopAddTask(evLoop, conn->channel, ADD);

	return conn;
}

int tcpConnectionDestroy(void* arg)
{
	struct TcpConnection* conn = (struct TcpConnection*)arg;
	if (conn != NULL)
	{
		if (conn->readBuf != NULL && bufferReadableSize(conn->readBuf)
			&& conn->writeBuf != NULL && bufferReadableSize(conn->writeBuf))
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
