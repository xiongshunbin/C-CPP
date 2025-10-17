#pragma once

#include "Buffer.h"
#include <string>
#include <map>
#include <functional>

// 定义状态码枚举
enum class HttpStatusCode : unsigned short
{
	Unknown = 0,
	OK = 200,				// 请求成功
	MovedPermanently = 301,	// 永久移动
	Found = 302,			// 临时移动
	BadRequest = 400,		// 客户端请求的语法错误，服务器无法理解
	NotFound = 404			// 服务器无法根据客户端的请求找到资源(网页)
};

// 定义Http响应类
class HttpResponse
{
public:
	HttpResponse();
	~HttpResponse();

	// 添加响应头
	void addHeader(std::string key, std::string value);

	// 组织HTTP响应报文
	void prepareMsg(Buffer* sendBuf, int socketFd);

	inline void setStatusCode(HttpStatusCode code) { m_statusCode = code; }
	inline void setFileName(std::string name) { m_fileName = name; }

private:
	// 状态行: HTTP版本, 状态码, 状态描述
	HttpStatusCode m_statusCode;
	// 响应头 - 键值对
	std::map<std::string, std::string> m_headers;
	// 回复数据的存储路径
	std::string m_fileName;
	// 定义状态码和状态描述的对应关系
	static const std::map<unsigned short, std::string> m_info;

public:
	// 定义一个可调用对象, 用来组织要回复给客户端的数据块
	std::function<void(const std::string&, Buffer*, int)> sendDataFun;
};
