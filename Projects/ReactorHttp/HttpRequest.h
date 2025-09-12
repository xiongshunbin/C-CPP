#pragma once

#include "Buffer.h"
#include <stdbool.h>

// 请求头键值对
struct RequestHeader
{
	char* key;
	char* value;
};

// 当前解析状态
enum HttpRequestState
{
	ParseReqLine,
	ParseReqHeaders,
	ParseReqBody,
	ParseReqDone
};

// 定义Http请求结构体
struct HttpRequest
{
	char* method;
	char* url;
	char* version;
	struct RequestHeader* reqHeaders;
	int reqHeadersNum;
	enum HttpRequestState curState;
};

// 初始化
struct HttpRequest* httpRequestInit();

// 重置
void httpRequestReset(struct HttpRequest* request);
void httpRequestResetEx(struct HttpRequest* request);
void httpRequestDestroy(struct HttpRequest* request);

// 获取处理状态
enum HttpRequestState httpRequestState(struct HttpRequest* request);

// 添加请求头
void httpRequestAddHeader(struct HttpRequest* request, const char* key, const char* value);

// 根据key得到请求头的value
char* httpRequestGetHeader(struct HttpRequest* request, const char* key);

// 解析请求行
bool parseHttpRequestLine(struct HttpRequest* request, struct Buffer* readBuf);

// 解析请求头
bool parseHttpRequestHeader(struct HttpRequest* request, struct Buffer* readBuf);

// 解析HTTP请求协议
bool parseHttpRequest(struct HttpRequest* request, struct Buffer* readBuf);

// 处理HTTP请求协议
bool processHttpRequest(struct HttpRequest* request);