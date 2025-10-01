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
	// ����������, �����ַ���������ַ
	char* end = bufferFindCRLF(readBuf);

	// �����ַ�����ʼ��ַ
	char* start = readBuf->data + readBuf->readPos;

	// �������ܳ���
	int lineSize = end - start;

	if (lineSize > 0)
	{
		// GET /books/bookid=10 HTTP/1.1

		start = splitRequestLine(start, end, " ", &(request->method));
		start = splitRequestLine(start, end, " ", &(request->url));
		splitRequestLine(start, end, NULL, &(request->version));
#if 0
		// ����ʽ
		char* space = memmem(start, lineSize, " ", 1);
		assert(space != NULL);
		int methodSize = space - start;
		request->method = (char*)malloc(sizeof(char) * (methodSize + 1));
		strncpy(request->method, start, methodSize);
		request->method[methodSize] = '\0';

		// ����ľ�̬��Դ
		start = space + 1;
		space = memmem(start, end - start, " ", 1);
		assert(space != NULL);
		int urlSize = space - start;
		request->url = (char*)malloc(sizeof(char) * (urlSize + 1));
		strncpy(request->url, start, urlSize);
		request->url[urlSize] = '\0';

		// HTTP �汾
		start = space + 1;
		int versionSize = end - start;
		request->version = (char*)malloc(sizeof(char) * (versionSize + 1));
		strncpy(request->version, start, versionSize);
		request->version[versionSize] = '\0';
#endif

		// Ϊ��������ͷ��׼��
		readBuf->readPos = readBuf->readPos + lineSize + strlen("\r\n");
		// �޸�״̬
		request->curState = ParseReqHeaders;
		return true;
	}

	return false;
}

/**
 * ��������: ��������ͷ�е�һ��
 */
bool parseHttpRequestHeader(struct HttpRequest* request, struct Buffer* readBuf)
{
	char* end = bufferFindCRLF(readBuf);
	if (end != NULL)
	{
		char* start = readBuf->data + readBuf->readPos;
		int lineSize = end - start;
		// ���� : �����ַ���
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

			// �ƶ������ݵ�λ��
			readBuf->readPos = readBuf->readPos + lineSize + strlen("\r\n");
		}
		else
		{
			// ����ͷ����������, ��������
			readBuf->readPos += strlen("\r\n");

			// �޸Ľ���״̬
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

		// �ж��Ƿ�������, ������, ��Ҫ׼���ظ�������
		if (request->curState == ParseReqDone)
		{
			// 1.���ݽ�������ԭʼ����, �Կͻ��˵�������������
			processHttpRequest(request, response);
			// 2.��֯��Ӧ���ݲ����͸��ͻ���
			httpResponsePrepareMsg(response, sendBuf, socketFd);
		}
	}
	request->curState = ParseReqLine;	// ״̬��ԭ, ��֤���ܼ�������ڶ������Ժ������
	return flag;
}

int processHttpRequest(struct HttpRequest* request, struct HttpResponse* response)
{
	// �������GET��HTTP����
	if (strcasecmp(request->method, "GET") != 0)	// ��GET����
	{
		return -1;
	}
	decodeMsg(request->url, request->url);
	// ����ͻ�������ľ�̬��Դ(Ŀ¼�����ļ�)
	const char* file = NULL;
	if (strcmp(request->url, "/") == 0)
	{
		file = "./";
	}
	else
	{
		file = request->url + 1;
	}
	// ��ȡ�ļ�����
	struct stat st;
	int ret = stat(file, &st);
	if (ret == -1)
	{
		// �ļ������� -- �ظ�404
		// ״̬��
		response->statusCode = NotFound;
		strcpy(response->statusMsg, "Not Found");

		// ��Ӧͷ
		httpResponseAddHeader(response, "content-type", getFileType(".html"));

		strcpy(response->fileName, "404.html");
		response->sendDataFun = sendFile;
		return 0;
	}

	response->statusCode = OK;
	strcpy(response->statusMsg, "OK");
	strcpy(response->fileName, file);
	// �ж��ļ�����
	if (S_ISDIR(st.st_mode))
	{
		// �����Ŀ¼�е����ݷ��͸��ͻ���
		httpResponseAddHeader(response, "content-type", getFileType(".html"));
		response->sendDataFun = sendDir;
	}
	else
	{
		// ���ļ������ݷ��͸��ͻ���
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
		// isxdigit -> �ж��ַ��ǲ���16���Ƹ�ʽ��ȡֵ�� 0-f
		// Linux%E5%86%85%E6%A0%B8.jpg
		if (from[0] == '%' && isxdigit(from[1]) && isxdigit(from[2]))
		{
			// ��16���Ƶ��� -> ʮ���� �������ֵ��ֵ�����ַ� int -> char
			// B2 == 178
			// ��3���ַ��������һ���ַ�������ַ�����ԭʼ����
			*to = hexToDec(from[1]) * 16 + hexToDec(from[2]);

			// ���� from[1] �� from[2]�� ��Ϊ�ڵ�ǰѭ�����Ѿ��������
			from += 2;
		}
		else
		{
			// �ַ���������ֵ
			*to = *from;
		}
	}
	*to = '\0';
}

const char* getFileType(const char* name)
{
	// a.jpg a.mp4 a.html
	// �����������'.'�ַ�����������ڷ���NULL
	const char* dot = strrchr(name, '.');
	if (dot == NULL)
		return "text/plain; charset=utf-8";	// ���ı�
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

	return "text/plain; charset=utf-8";	// ���ı�
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
		// ȡ���ļ��� namelist ָ�����һ��ָ������ struct durent* tmp[]
		const char* name = namelist[i]->d_name;
		struct stat st;
		char subPath[1024] = { 0 };
		sprintf(subPath, "%s/%s", dirName, name);
		stat(subPath, &st);
		if (S_ISDIR(st.st_mode))
		{
			// a��ǩ <a href="">name</a>
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
	// 1. ���ļ�
	int fd = open(fileName, O_RDONLY);
	assert(fd > 0);
	// 2. ���ļ��е����ݷ��͵��ͻ���
#if 1
	char buf[1024];
	while (1)
	{
		bzero(buf, sizeof(buf));
		int len = read(fd, buf, sizeof buf);
		if (len > 0)
		{
			bufferAppendData(sendBuf, buf, len);
			usleep(10);	// ��Ҫ������
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
