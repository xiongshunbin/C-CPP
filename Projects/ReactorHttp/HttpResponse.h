#pragma once

#include "Buffer.h"

// ����״̬��ö��
enum HttpStatusCode
{
	UnKnown = 0,
	OK = 200,
	MovedPermanently = 301,
	MovedTemporarily = 302,
	BadRequest = 400,
	NotFound = 404
};

// ������Ӧͷ��ֵ��
struct ResponseHeader
{
	char key[32];
	char value[128];
};

// ����һ������ָ��, ������֯Ҫ�ظ����ͻ��˵����ݿ�
typedef void (*ResponseBody)(const char* fileName, struct Buffer* sendBuf, int socketFd);

// ����ṹ��
struct HttpResponse
{
	// ״̬��: HTTP�汾, ״̬��, ״̬����,
	enum HttpStatusCode statusCode;
	char statusMsg[128];
	// ��Ӧͷ - ��ֵ��
	struct ResponseHeader* headers;
	int headerNum;
	// �ظ����ݵĴ洢·��
	char fileName[128];
	ResponseBody sendDataFun;
};

// ��ʼ��
struct HttpResponse* httpResponseInit();

// ����
void httpResponseDestroy(struct HttpResponse* response);

// �����Ӧͷ
void httpResponseAddHeader(struct HttpResponse* response, const char* key, const char* value);

// ��֯HTTP��Ӧ����
void httpResponsePrepareMsg(struct HttpResponse* response, struct Buffer* sendBuf, int socketFd);
