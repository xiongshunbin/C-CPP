#pragma once

#include "Buffer.h"
#include "HttpResponse.h"
#include <cstdint>
#include <string>
#include <map>
#include <functional>

// 当前解析状态
enum class ParseState : uint8_t
{
	ParseReqLine,
	ParseReqHeaders,
	ParseReqBody,
	ParseReqDone
};

// 定义Http请求类
class HttpRequest
{
public:
	HttpRequest();
	~HttpRequest();

	// 重置
	void reset();

	// 添加请求头
	void addHeader(const std::string key, const std::string value);

	// 根据key得到请求头的value
	std::string getHeader(const std::string& key);

	// 解析请求行
	bool parseHttpRequestLine(Buffer* readBuf);

	// 解析请求头
	bool parseHttpRequestHeader(Buffer* readBuf);

	// 解析HTTP请求协议
	bool parseHttpRequest(Buffer* readBuf, HttpResponse* response, Buffer* sendBuf, int socketFd);

	// 处理HTTP请求协议
	int processHttpRequest(HttpResponse* response);

	// 解码字符串
	std::string decodeMsg(std::string& msg);

	// 根据文件名获取文件类型
	std::string getFileType(const std::string& name);

	// 发送目录
	static void sendDir(const std::string& dirName, Buffer* sendBuf, int cfd);

	// 发送文件
	static void sendFile(const std::string& fileName, Buffer* sendBuf, int cfd);

	// 获取/设置处理状态
	inline ParseState getParseState() const { return m_curState; }
	inline void setParseState(ParseState state) { m_curState = state; }

private:
	inline void setMethod(std::string method) { m_method = method; }
	inline void setURL(std::string url) { m_url = url; }
	inline void setVersion(std::string version) { m_version = version; }

	char* splitRequestLine(const char* start, const char* end, 
		const char* sub, std::function<void(std::string)> callback);

	int hexToDec(char c);

private:
	std::string m_method;
	std::string m_url;
	std::string m_version;
	std::map<std::string, std::string> m_reqHeaders;
	ParseState m_curState;
};
