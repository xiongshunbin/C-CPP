#include "HttpResponse.h"
#include "TcpConnection.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <strings.h>

const std::map<unsigned short, std::string> HttpResponse::m_info = {
	{0, "Unknown"},
	{200, "OK"},
	{301, "Moved Permanently"},
	{302, "Found"},
	{400, "Bad Request"},
	{404, "Not Found"}
};

HttpResponse::HttpResponse() : m_statusCode(HttpStatusCode::Unknown)
{
	m_headers.clear();
	m_fileName = std::string();
	sendDataFun = nullptr;
}

HttpResponse::~HttpResponse()
{
}

void HttpResponse::addHeader(std::string key, std::string value)
{
	if (key.empty() || value.empty())
	{
		return;
	}
	m_headers.insert(std::make_pair(key, value));
}

void HttpResponse::prepareMsg(Buffer* sendBuf, int socketFd)
{
	// 1.状态行
	char temp[1024] = { 0 };
	unsigned short code = static_cast<unsigned short>(m_statusCode);
	sprintf(temp, "HTTP/1.1 %d %s\r\n", code, m_info.at(code).data());
	sendBuf->appendString(temp);

	// 2.响应头
	for (auto it = m_headers.begin(); it != m_headers.end(); it++)
	{
		bzero(temp, sizeof((temp)));
		sprintf(temp, "%s: %s\r\n", it->first.data(), it->second.data());
		sendBuf->appendString(temp);
	}

	// 3.空行
	sendBuf->appendString("\r\n");
#ifndef MSG_SEND_AUTO
	sendBuf->sendData(socketFd);
#endif

	// 4.回复的数据
	sendDataFun(m_fileName, sendBuf, socketFd);
}
