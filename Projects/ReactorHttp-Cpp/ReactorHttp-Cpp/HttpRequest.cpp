#include "HttpRequest.h"
#include "TcpConnection.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <strings.h>
#include <assert.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

HttpRequest::HttpRequest()
{
	reset();
}

HttpRequest::~HttpRequest()
{
}

void HttpRequest::reset()
{
	m_curState = ParseState::ParseReqLine;
	m_method = m_url = m_version = std::string();	// ""
	m_reqHeaders.clear();
}

void HttpRequest::addHeader(const std::string key, const std::string value)
{
	if (key.empty() || value.empty())
	{
		return;
	}
	m_reqHeaders.insert(std::make_pair(key, value));
}

std::string HttpRequest::getHeader(const std::string& key)
{
	auto item = m_reqHeaders.find(key);
	if (item == m_reqHeaders.end())
	{
		return std::string();
	}
	return item->second;
}

bool HttpRequest::parseHttpRequestLine(Buffer* readBuf)
{
	// 读出请求行, 保存字符串结束地址
	char* end = const_cast<char*>(readBuf->findCRLF());

	// 保存字符串起始地址
	char* start = const_cast<char*>(readBuf->data());

	// 请求行总长度
	int lineSize = end - start;

	if (lineSize > 0)
	{
		// GET /books/bookid=10 HTTP/1.1
		auto methodFunc = std::bind(&HttpRequest::setMethod, this, std::placeholders::_1);
		auto urlFunc = std::bind(&HttpRequest::setURL, this, std::placeholders::_1);
		auto versionFunc = std::bind(&HttpRequest::setVersion, this, std::placeholders::_1);
		start = splitRequestLine(start, end, " ", methodFunc);
		start = splitRequestLine(start, end, " ", urlFunc);
		splitRequestLine(start, end, nullptr, versionFunc);

		// 为解析请求头做准备
		readBuf->readPosIncrease(lineSize + strlen("\r\n"));
		// 修改状态
		setParseState(ParseState::ParseReqHeaders);
		return true;
	}
	return false;
}

/**
 * 函数功能: 处理请求头中的一行
 */
bool HttpRequest::parseHttpRequestHeader(Buffer* readBuf)
{
	char* end = const_cast<char*>(readBuf->findCRLF());
	if (end != nullptr)
	{
		char* start = const_cast<char*>(readBuf->data());
		int lineSize = end - start;
		// 基于 : 搜索字符串
		char* middle = static_cast<char*>(memmem(start, lineSize, ": ", 2));
		if (middle != nullptr)
		{
			int keyLen = middle - start;
			int valueLen = end - middle - strlen(": ");
			if (keyLen > 0 && valueLen > 0)
			{
				std::string key(start, keyLen);
				std::string value(middle + strlen(": "), valueLen);
				addHeader(key, value);
			}
			// 移动读数据的位置
			readBuf->readPosIncrease(lineSize + strlen("\r\n"));
		}
		else
		{
			// 请求头被解析完了, 跳过空行
			readBuf->readPosIncrease(strlen("\r\n"));

			// 修改解析状态
			if (strcasecmp(m_method.data(), "POST") == 0)
			{
				setParseState(ParseState::ParseReqBody);
			}
			else
			{
				setParseState(ParseState::ParseReqDone);
			}
		}
		return true;
	}
	return false;
}

bool HttpRequest::parseHttpRequest(Buffer* readBuf, HttpResponse* response, Buffer* sendBuf, int socketFd)
{
	bool flag = true;
	while (m_curState != ParseState::ParseReqDone)
	{
		switch (m_curState)
		{
		case ParseState::ParseReqLine:
			flag = parseHttpRequestLine(readBuf);
			break;
		case ParseState::ParseReqHeaders:
			flag = parseHttpRequestHeader(readBuf);
			break;
		case ParseState::ParseReqBody:
			break;
		default:
			break;
		}

		if (!flag)
		{
			return false;
		}

		// 判断是否解析完毕, 如果完毕, 需要准备回复的数据
		if (m_curState == ParseState::ParseReqDone)
		{
			// 1.根据解析出的原始数据, 对客户端的请求做出处理
			processHttpRequest(response);
			// 2.组织响应数据并发送给客户端
			response->prepareMsg(sendBuf, socketFd);
		}
	}
	setParseState(ParseState::ParseReqLine);	// 状态还原, 保证还能继续处理第二条及以后的请求
	return flag;
}

int HttpRequest::processHttpRequest(HttpResponse* response)
{
	// 处理基于GET的HTTP请求
	if (strcasecmp(m_method.data(), "GET") != 0)	// 非GET请求
	{
		return -1;
	}
	m_url = decodeMsg(m_url);
	// 处理客户端请求的静态资源(目录或者文件)
	const char* file = NULL;
	if (strcmp(m_url.data(), "/") == 0)
	{
		file = "./";
	}
	else
	{
		file = m_url.data() + 1;
	}
	// 获取文件属性
	struct stat st;
	int ret = stat(file, &st);
	if (ret == -1)
	{
		// 文件不存在 -- 回复404
		// 状态行
		response->setStatusCode(HttpStatusCode::NotFound);
		// 响应头
		response->addHeader("content-type", getFileType(".html"));
		// 响应体
		response->setFileName("404.html");
		response->sendDataFun = sendFile;
		return 0;
	}

	response->setStatusCode(HttpStatusCode::OK);
	response->setFileName(file);
	// 判断文件类型
	if (S_ISDIR(st.st_mode))
	{
		// 把这个目录中的内容发送给客户端
		response->addHeader("content-type", getFileType(".html"));
		response->sendDataFun = sendDir;
	}
	else
	{
		// 把文件的内容发送给客户端
		response->addHeader("content-type", getFileType(file));
		response->addHeader("content-length", std::to_string(st.st_size));
		response->sendDataFun = sendFile;
	}
	return 0;
}

std::string HttpRequest::decodeMsg(std::string& msg)
{
	std::string str = std::string();
	const char* from = msg.data();
	for (; *from != '\0'; ++from)
	{
		// isxdigit -> 判断字符是不是16进制格式，取值在 0-f
		// Linux%E5%86%85%E6%A0%B8.jpg
		if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
		{
			// 将16进制的数 -> 十进制 将这个数值赋值给了字符 int -> char
			// B2 == 178
			// 将3个字符，变成了一个字符，这个字符就是原始数据
			str.append(1, hexToDec(from[1]) * 16 + hexToDec(from[2]));

			// 跳过 from[1] 和 from[2]， 因为在当前循环中已经处理过了
			from += 2;
		}
		else
		{
			// 字符拷贝，赋值
			str.append(1, *from);
		}
	}
	str.append(1, '\0');
	return str;
}

std::string HttpRequest::getFileType(const std::string& name)
{
	// a.jpg a.mp4 a.html
	// 自右向左查找'.'字符，如果不存在返回NULL
	const char* dot = strrchr(name.data(), '.');
	if (dot == NULL)
		return "text/plain; charset=utf-8";	// 纯文本
	if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
		return "text/html; charset=utf-8";
	if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
		return "image/jpeg";
	if (strcmp(dot, ".gif") == 0)
		return "image/gif";
	if (strcmp(dot, ".png") == 0)
		return "image/png";
	if (strcmp(dot, ".css") == 0)
		return "text/css";
	if (strcmp(dot, ".au") == 0)
		return "audio/basic";
	if (strcmp(dot, ".wav") == 0)
		return "audio/wav";
	if (strcmp(dot, ".avi") == 0)
		return "video/x-msvideo";
	if (strcmp(dot, ".mov") == 0 || strcmp(dot, ".qt") == 0)
		return "video/quicktime";
	if (strcmp(dot, ".mpeg") == 0 || strcmp(dot, ".mpe") == 0)
		return "video/mpeg";
	if (strcmp(dot, ".wrl") == 0)
		return "model/vrml";
	if (strcmp(dot, ".midi") == 0 || strcmp(dot, ".mid") == 0)
		return "audio/midi";
	if (strcmp(dot, ".mp3") == 0)
		return "audio/mp3";
	if (strcmp(dot, ".json") == 0)
		return "application/json";
	if (strcmp(dot, ".pdf") == 0)
		return "application/pdf";

	return "text/plain; charset=utf-8";	// 纯文本
}

/*
<html>
	<head>
		<title>test</title>
	</head>
	<body>
		<table>
			<tr>
				<td></td>
				<td></td>
			</tr>
			<tr>
				<td></td>
				<td></td>
			</tr>
		</table>
	</body>
</html>
*/
void HttpRequest::sendDir(const std::string& dirName, Buffer* sendBuf, int cfd)
{
	char buf[4096] = { 0 };
	sprintf(buf, "<html><head><title>%s</title></head><body><table>", dirName.data());
	struct dirent** namelist;
	int num = scandir(dirName.data(), &namelist, NULL, alphasort);
	for (int i = 0; i < num; i++)
	{
		// 取出文件名 namelist 指向的是一个指针数组 struct durent* tmp[]
		const char* name = namelist[i]->d_name;
		struct stat st;
		char subPath[1024] = { 0 };
		sprintf(subPath, "%s/%s", dirName.data(), name);
		stat(subPath, &st);
		if (S_ISDIR(st.st_mode))
		{
			// a标签 <a href="">name</a>
			sprintf(buf + strlen(buf), "<tr><td><a href=\"%s/\">%s</a></td><td>%ld</td></tr>", name, name, st.st_size);
		}
		else
		{
			sprintf(buf + strlen(buf), "<tr><td><a href=\"%s\">%s</a></td><td>%ld</td></tr>", name, name, st.st_size);
		}
		sendBuf->appendString(buf);
#ifndef MSG_SEND_AUTO
		sendBuf->sendData(cfd);
#endif
		memset(buf, 0, sizeof(buf));
		free(namelist[i]);
	}
	sprintf(buf, "</table></body></html>");
	sendBuf->appendString(buf);
#ifndef MSG_SEND_AUTO
	sendBuf->sendData(cfd);
#endif
	free(namelist);
}

void HttpRequest::sendFile(const std::string& fileName, Buffer* sendBuf, int cfd)
{
	// 1. 打开文件
	int fd = open(fileName.data(), O_RDONLY);
	assert(fd > 0);
	// 2. 把文件中的内容发送到客户端
#if 1
	char buf[1024];
	while (1)
	{
		bzero(buf, sizeof(buf));
		int len = read(fd, buf, sizeof buf);
		if (len > 0)
		{
			sendBuf->appendString(buf, len);
#ifndef MSG_SEND_AUTO
			sendBuf->sendData(cfd);
#endif
		}
		else if (len == 0)
		{
			break;
		}
		else
		{
			close(fd);
			perror("read");
		}
	}
#else
	off_t offset = 0;
	int size = lseek(fd, 0, SEEK_END);
	lseek(fd, 0, SEEK_SET);
	while (offset < size)
	{
		int ret = sendfile(cfd, fd, &offset, size - offset);
		printf("ret value: %d\n", ret);
		if (ret == -1 && errno == EAGAIN)
		{
			printf("There's no data......\n");
		}
	}
#endif
	close(fd);
}

char* HttpRequest::splitRequestLine(const char* start, const char* end, 
	const char* sub, std::function<void(std::string)> callback)
{
	char* space = const_cast<char*>(end);
	if (sub != nullptr)
	{
		space = static_cast<char*>(memmem(start, end - start, sub, strlen(sub)));
		assert(space != nullptr);
	}
	int length = space - start;
	callback(std::string(start, length));
	return space != end ? (space + strlen(sub)) : const_cast<char*>(end);
}

// 将字符转换为整形数
int HttpRequest::hexToDec(char c)
{
	if (c >= '0' && c <= '9')
	{
		return c - '0';
	}
	else if (c >= 'a' && c <= 'f')
	{
		return c - 'a' + 10;
	}
	else if (c >= 'A' && c <= 'F')
	{
		return c - 'A' + 10;
	}

	return 0;
}
