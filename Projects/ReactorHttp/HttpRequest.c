#include "HttpRequest.h"
#include <stdlib.h>
#include <strings.h>
#include <string.h>

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
