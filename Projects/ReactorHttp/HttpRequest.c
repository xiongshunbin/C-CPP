#include "HttpRequest.h"
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <unistd.h>

#define HeaderSize 12

struct HttpRequest* httpRequestInit()
{
	struct HttpRequest* request = (struct HttpRequest*)malloc(sizeof(struct HttpRequest));
	httpRequestReset(request);
	request->reqHeaders = (struct RequestHeader*)malloc(sizeof(struct RequestHeader) * HeaderSize);
	return request;
}

void httpRequestReset(struct HttpRequest* request)
{
	request->curState = ParseReqLine;
	request->method = NULL;
	request->url = NULL;
	request->version = NULL;
	request->reqHeadersNum = 0;
}

void httpRequestResetEx(struct HttpRequest* request)
{
	free(request->method);
	free(request->url);
	free(request->version);
	if (request->reqHeaders != NULL)
	{
		for (int i = 0; i < request->reqHeadersNum; i++)
		{
			free(request->reqHeaders[i].key);
			free(request->reqHeaders[i].value);
		}
		free(request->reqHeaders);
	}
	httpRequestReset(request);
}

void httpRequestDestroy(struct HttpRequest* request)
{
	if (request != NULL)
	{
		httpRequestResetEx(request);
		free(request);
	}
}

enum HttpRequestState httpRequestState(struct HttpRequest* request)
{
	return request->curState;
}

void httpRequestAddHeader(struct HttpRequest* request, const char* key, const char* value)
{
	if (request->reqHeadersNum < HeaderSize)
	{
		request->reqHeaders[request->reqHeadersNum].key = key;
		request->reqHeaders[request->reqHeadersNum].value = value;
		request->reqHeadersNum++;
	}
}

char* httpRequestGetHeader(struct HttpRequest* request, const char* key)
{
	if (request != NULL && request->reqHeaders != NULL)
	{
		for (int i = 0; i < request->reqHeadersNum; i++)
		{
			if (strncasecmp(request->reqHeaders[i].key, key, strlen(key)) == 0)
			{
				return request->reqHeaders[i].value;
			}
		}
	}
	return NULL;
}

char* splitRequestLine(const char* start, const char* end, const char* sub, char** ptrDest)
{
	char* space = end;
	if (sub != NULL)
	{
		space = memmem(start, end - start, sub, strlen(sub));
		assert(space != NULL);
	}
	int length = space - start;
	char* temp = (char*)malloc(sizeof(char) * (length + 1));
	strncpy(temp, start, length);
	temp[length] = '\0';
	*ptrDest = temp;
	return space != end ? (space + strlen(sub)) : end;
}

bool parseHttpRequestLine(struct HttpRequest* request, struct Buffer* readBuf)
{
	// 读出请求行, 保存字符串结束地址
	char* end = bufferFindCRLF(readBuf);

	// 保存字符串起始地址
	char* start = readBuf->data + readBuf->readPos;

	// 请求行总长度
	int lineSize = end - start;

	if (lineSize > 0)
	{
		// GET /books/bookid=10 HTTP/1.1

		start = splitRequestLine(start, end, " ", &(request->method));
		start = splitRequestLine(start, end, " ", &(request->url));
		splitRequestLine(start, end, NULL, &(request->version));
#if 0
		// 请求方式
		char* space = memmem(start, lineSize, " ", 1);
		assert(space != NULL);
		int methodSize = space - start;
		request->method = (char*)malloc(sizeof(char) * (methodSize + 1));
		strncpy(request->method, start, methodSize);
		request->method[methodSize] = '\0';

		// 请求的静态资源
		start = space + 1;
		space = memmem(start, end - start, " ", 1);
		assert(space != NULL);
		int urlSize = space - start;
		request->url = (char*)malloc(sizeof(char) * (urlSize + 1));
		strncpy(request->url, start, urlSize);
		request->url[urlSize] = '\0';

		// HTTP 版本
		start = space + 1;
		int versionSize = end - start;
		request->version = (char*)malloc(sizeof(char) * (versionSize + 1));
		strncpy(request->version, start, versionSize);
		request->version[versionSize] = '\0';
#endif

		// 为解析请求头做准备
		readBuf->readPos = readBuf->readPos + lineSize + strlen("\r\n");
		// 修改状态
		request->curState = ParseReqHeaders;
		return true;
	}

	return false;
}

/**
 * 函数功能: 处理请求头中的一行
 */
bool parseHttpRequestHeader(struct HttpRequest* request, struct Buffer* readBuf)
{
	char* end = bufferFindCRLF(readBuf);
	if (end != NULL)
	{
		char* start = readBuf->data + readBuf->readPos;
		int lineSize = end - start;
		// 基于 : 搜索字符串
		char* middle = memmem(start, lineSize, ": ", 2);
		if (middle != NULL)
		{
			int keySize = middle - start;
			char* key = (char*)malloc(sizeof(char) * (keySize + 1));
			strncpy(key, start, keySize);
			key[keySize] = '\0';

			start = middle + strlen(": ");
			int valueSize = end - start;
			char* value = (char*)malloc(sizeof(char) * (valueSize + 1));
			strncpy(value, start, valueSize);
			value[valueSize] = '\0';

			httpRequestAddHeader(request, key, value);

			// 移动读数据的位置
			readBuf->readPos = readBuf->readPos + lineSize + strlen("\r\n");
		}
		else
		{
			// 请求头被解析完了, 跳过空行
			readBuf->readPos += strlen("\r\n");

			// 修改解析状态
			if (strcasecmp(request->method, "POST") == 0)
			{
				request->curState = ParseReqBody;
			}
			else
			{
				request->curState = ParseReqDone;
			}
		}
		return true;
	}
	return false;
}

bool parseHttpRequest(struct HttpRequest* request, struct Buffer* readBuf,
	struct HttpResponse* response, struct Buffer* sendBuf, int socketFd)
{
	bool flag = true;
	while (request->curState != ParseReqDone)
	{
		switch (request->curState)
		{
		case ParseReqLine:
			flag = parseHttpRequestLine(request, readBuf);
			break;
		case ParseReqHeaders:
			flag = parseHttpRequestHeader(request, readBuf);
			break;
		case ParseReqBody:
			break;
		default:
			break;
		}

		if (!flag)
		{
			return false;
		}

		// 判断是否解析完毕, 如果完毕, 需要准备回复的数据
		if (request->curState == ParseReqDone)
		{
			// 1.根据解析出的原始数据, 对客户端的请求做出处理
			processHttpRequest(request, response);
			// 2.组织响应数据并发送给客户端
			httpResponsePrepareMsg(response, sendBuf, socketFd);
		}
	}
	request->curState = ParseReqLine;	// 状态还原, 保证还能继续处理第二条及以后的请求
	return flag;
}

int processHttpRequest(struct HttpRequest* request, struct HttpResponse* response)
{
	// 处理基于GET的HTTP请求
	if (strcasecmp(request->method, "GET") != 0)	// 非GET请求
	{
		return -1;
	}
	decodeMsg(request->url, request->url);
	// 处理客户端请求的静态资源(目录或者文件)
	const char* file = NULL;
	if (strcmp(request->url, "/") == 0)
	{
		file = "./";
	}
	else
	{
		file = request->url + 1;
	}
	// 获取文件属性
	struct stat st;
	int ret = stat(file, &st);
	if (ret == -1)
	{
		// 文件不存在 -- 回复404
		// 状态行
		response->statusCode = NotFound;
		strcpy(response->statusMsg, "Not Found");

		// 响应头
		httpResponseAddHeader(response, "content-type", getFileType(".html"));

		strcpy(response->fileName, "404.html");
		response->sendDataFun = sendFile;
		return 0;
	}

	response->statusCode = OK;
	strcpy(response->statusMsg, "OK");
	strcpy(response->fileName, file);
	// 判断文件类型
	if (S_ISDIR(st.st_mode))
	{
		// 把这个目录中的内容发送给客户端
		httpResponseAddHeader(response, "content-type", getFileType(".html"));
		response->sendDataFun = sendDir;
	}
	else
	{
		// 把文件的内容发送给客户端
		httpResponseAddHeader(response, "content-type", getFileType(file));
		char temp[32] = { 0 };
		sprintf(temp, "%ld", st.st_size);
		httpResponseAddHeader(response, "content-length", temp);
		response->sendDataFun = sendFile;
	}
	return 0;
}

int hexToDec(char c)
{
	if (c >= '0' && c <= '9')
		return c - '0';
	if (c >= 'a' && c <= 'f')
		return c - 'a' + 10;
	if (c >= 'A' && c <= 'F')
		return c - 'A' + 10;

	return 0;
}

void decodeMsg(char* to, char* from)
{
	for (; *from != '\0'; ++to, ++from)
	{
		// isxdigit -> 判断字符是不是16进制格式，取值在 0-f
		// Linux%E5%86%85%E6%A0%B8.jpg
		if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
		{
			// 将16进制的数 -> 十进制 将这个数值赋值给了字符 int -> char
			// B2 == 178
			// 将3个字符，变成了一个字符，这个字符就是原始数据
			*to = hexToDec(from[1]) * 16 + hexToDec(from[2]);

			// 跳过 from[1] 和 from[2]， 因为在当前循环中已经处理过了
			from += 2;
		}
		else
		{
			// 字符拷贝，赋值
			*to = *from;
		}
	}
	*to = '\0';
}

const char* getFileType(const char* name)
{
	// a.jpg a.mp4 a.html
	// 自右向左查找'.'字符，如果不存在返回NULL
	const char* dot = strrchr(name, '.');
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
void sendDir(const char* dirName, struct Buffer* sendBuf, int cfd)
{
	char buf[4096] = { 0 };
	sprintf(buf, "<html><head><title>%s</title></head><body><table>", dirName);
	struct dirent** namelist;
	int num = scandir(dirName, &namelist, NULL, alphasort);
	for (int i = 0; i < num; i++)
	{
		// 取出文件名 namelist 指向的是一个指针数组 struct durent* tmp[]
		const char* name = namelist[i]->d_name;
		struct stat st;
		char subPath[1024] = { 0 };
		sprintf(subPath, "%s/%s", dirName, name);
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
		bufferAppendString(sendBuf, buf);
		memset(buf, 0, sizeof(buf));
		free(namelist[i]);
	}
	sprintf(buf, "</table></body></html>");
	bufferAppendString(sendBuf, buf);
	free(namelist);
}

void sendFile(const char* fileName, struct Buffer* sendBuf, int cfd)
{
	// 1. 打开文件
	int fd = open(fileName, O_RDONLY);
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
			bufferAppendData(sendBuf, buf, len);
			usleep(10);	// 重要！！！
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
