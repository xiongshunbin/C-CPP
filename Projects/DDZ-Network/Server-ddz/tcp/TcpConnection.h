#pragma once

#include "EventLoop.h"
#include "Buffer.h"
#include "Channel.h"
#include "HttpRequest.h"
#include "HttpResponse.h"

//#define MSG_SEND_AUTO

class TcpConnection
{
public:
	TcpConnection(int fd, EventLoop* evLoop);
	~TcpConnection();

private:
	static int processRead(void* arg);
	static int processWrite(void* arg);
	static int destroy(void* arg);

private:
	std::string m_name;
	EventLoop* m_evLoop = nullptr;
	Channel* m_channel = nullptr;
	Buffer* m_readBuf = nullptr;
	Buffer* m_writeBuf = nullptr;
	// http 协议
	HttpRequest* m_request = nullptr;
	HttpResponse* m_response = nullptr;
};
