#include "HttpResponse.h"
#include <stdlib.h>
#include <strings.h>
#include <string.h>

#define resHeaderSize 16

struct HttpResponse* httpResponseInit()
{
	struct HttpResponse* response = (struct HttpResponse*)malloc(sizeof(struct HttpResponse));
	response->statusCode = UnKnown;
	bzero(response->statusMsg, sizeof(response->statusMsg));
	response->headers = (struct ResponseHeader*)malloc(sizeof(struct ResponseHeader) * resHeaderSize);
	response->headerNum = 0;
	// 初始化数组
	bzero(response->headers, sizeof(struct ResponseHeader) * resHeaderSize);
	bzero(response->fileName, sizeof(response->fileName));
	// 函数指针
	response->sendDataFun = NULL;

	return response;
}

void httpResponseDestroy(struct HttpResponse* response)
{
	if (response != NULL)
	{
		free(response->headers);
		response->headers = NULL;
		free(response);
		response = NULL;
	}
}

void httpResponseAddHeader(struct HttpResponse* response, const char* key, const char* value)
{
	if (response == NULL || key == NULL || value == NULL)
	{
		return;
	}

	struct ResponseHeader header;
	strcpy(response->headers[response->headerNum].key, key);
	strcpy(response->headers[response->headerNum].value, value);
	response->headerNum++;
}

void httpResponsePrepareMsg(struct HttpResponse* response, struct Buffer* sendBuf, int socketFd)
{
	// 1.状态行
	char temp[1024] = { 0 };
	sprintf(temp, "HTTP/1.1 %d %s\r\n", response->statusCode, response->statusMsg);
	bufferAppendString(sendBuf, temp);

	// 2.响应头
	for (int i = 0; i < response->headerNum; i++)
	{
		bzero(temp, sizeof((temp)));
		sprintf(temp, "%s: %s\r\n", response->headers[i].key, response->headers[i].value);
		bufferAppendString(sendBuf, temp);
	}

	// 3.空行
	bufferAppendString(sendBuf, "\r\n");

	// 4.回复的数据
	response->sendDataFun(response->fileName, sendBuf, socketFd);
}
