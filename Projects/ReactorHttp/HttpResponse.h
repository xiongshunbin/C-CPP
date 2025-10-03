#pragma once

#include "Buffer.h"

// 定义状态码枚举
enum HttpStatusCode
{
	UnKnown = 0,
	OK = 200,
	MovedPermanently = 301,
	MovedTemporarily = 302,
	BadRequest = 400,
	NotFound = 404
};

// 定义响应头键值对
struct ResponseHeader
{
	char key[32];
	char value[128];
};

// 定义一个函数指针, 用来组织要回复给客户端的数据块
typedef void (*ResponseBody)(const char* fileName, struct Buffer* sendBuf, int socketFd);

// 定义结构体
struct HttpResponse
{
	// 状态行: HTTP版本, 状态码, 状态描述,
	enum HttpStatusCode statusCode;
	char statusMsg[128];
	// 响应头 - 键值对
	struct ResponseHeader* headers;
	int headerNum;
	// 回复数据的存储路径
	char fileName[128];
	ResponseBody sendDataFun;
};

// 初始化
struct HttpResponse* httpResponseInit();

// 销毁
void httpResponseDestroy(struct HttpResponse* response);

// 添加响应头
void httpResponseAddHeader(struct HttpResponse* response, const char* key, const char* value);

// 组织HTTP响应报文
void httpResponsePrepareMsg(struct HttpResponse* response, struct Buffer* sendBuf, int socketFd);
