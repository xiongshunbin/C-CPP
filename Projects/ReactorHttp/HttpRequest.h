#pragma once

#include "Buffer.h"
#include <stdbool.h>
#include "HttpResponse.h"

// ����ͷ��ֵ��
struct RequestHeader
{
	char* key;
	char* value;
};

// ��ǰ����״̬
enum HttpRequestState
{
	ParseReqLine,
	ParseReqHeaders,
	ParseReqBody,
	ParseReqDone
};

// ����Http����ṹ��
struct HttpRequest
{
	char* method;
	char* url;
	char* version;
	struct RequestHeader* reqHeaders;
	int reqHeadersNum;
	enum HttpRequestState curState;
};

// ��ʼ��
struct HttpRequest* httpRequestInit();

// ����
void httpRequestReset(struct HttpRequest* request);
void httpRequestResetEx(struct HttpRequest* request);
void httpRequestDestroy(struct HttpRequest* request);

// ��ȡ����״̬
enum HttpRequestState httpRequestState(struct HttpRequest* request);

// �������ͷ
void httpRequestAddHeader(struct HttpRequest* request, const char* key, const char* value);

// ����key�õ�����ͷ��value
char* httpRequestGetHeader(struct HttpRequest* request, const char* key);

// ����������
bool parseHttpRequestLine(struct HttpRequest* request, struct Buffer* readBuf);

// ��������ͷ
bool parseHttpRequestHeader(struct HttpRequest* request, struct Buffer* readBuf);

// ����HTTP����Э��
bool parseHttpRequest(struct HttpRequest* request, struct Buffer* readBuf, 
	struct HttpResponse* response, struct Buffer* sendBuf, int socketFd);

// ����HTTP����Э��
int processHttpRequest(struct HttpRequest* request, struct HttpResponse* response);

// �����ַ���
void decodeMsg(char* to, char* from);

// �����ļ�����ȡ�ļ�����
const char* getFileType(const char* name);