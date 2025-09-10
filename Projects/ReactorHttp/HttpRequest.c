#include "HttpRequest.h"
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <assert.h>

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
			if (strncasecmp(request->method, "GET", strlen("GET")))
			{
				request->curState = ParseReqDone;
			}
			else if (strncasecmp(request->method, "POST", strlen("POST")))
			{
				request->curState = ParseReqBody;
			}
		}
		return true;
	}
	return false;
}
