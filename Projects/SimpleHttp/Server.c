#include "Server.h"
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <strings.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <assert.h>
#include <sys/sendfile.h>
#include <dirent.h>
#include <stdlib.h>
#include <ctype.h>
#include <pthread.h>

struct FdInfo {
	int fd;
	int epfd;
	pthread_t tid;
};

int initListenFd(unsigned short port)
{
	// 1. 创建用于监听的套接字
	int lfd = socket(AF_INET, SOCK_STREAM, 0);
	if (lfd == -1)
	{
		perror("socket");
		return -1;
	}

	// 2. 设置端口复用
	int opt = 1;
	int ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);
	if (ret == -1)
	{
		perror("setsockopt");
		return -1;
	}

	// 3. 绑定IP地址和端口号
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);
	ret = bind(lfd, (struct sockaddr*)&addr, sizeof addr);
	if (ret == -1)
	{
		perror("bind");
		return -1;
	}

	// 4. 设置监听
	ret = listen(lfd, 128);
	if (ret == -1)
	{
		perror("listen");
		return -1;
	}

	// 5. 返回初始化好的用于监听的套接字
	return lfd;
}

int epollRun(int lfd)
{
	// 1. 创建epoll树
	int epfd = epoll_create(1);
	if (epfd == -1)
	{
		perror("epoll_create");
		return -1;
	}

	// 2. 添加listenFd到epoll树
	struct epoll_event ev;
	ev.events = EPOLLIN;
	ev.data.fd = lfd;
	int ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
	if (ret == -1)
	{
		perror("epoll_ctl");
		return -1;
	}

	// 3. 监测事件
	struct epoll_event evs[1024];
	int maxevs = sizeof(evs) / sizeof(struct epoll_event);
	while (1)
	{
		int nfds = epoll_wait(epfd, evs, sizeof maxevs, -1);
		for (int i = 0; i < nfds; i++)
		{
			struct FdInfo* info = (struct FdInfo*)malloc(sizeof(struct FdInfo));
			int fd = evs[i].data.fd;
			info->epfd = epfd;
			info->fd = fd;
			if (fd == lfd)
			{
				// 建立新连接
				// acceptClient(lfd, epfd);
				pthread_create(&info->tid, NULL, acceptClient, info);
			}
			else
			{
				// 数据通信，主要是接受对端发送的数据
				// recvHttpRequest(fd, epfd);
				pthread_create(&info->tid, NULL, recvHttpRequest, info);
			}

		}
	}

	return 0;
}

void* acceptClient(void* arg)
{
	struct FdInfo* info = (struct FdInfo*)arg;
	// 1. 建立连接
	int cfd = accept(info->fd, NULL, NULL);
	if (cfd == -1)
	{
		perror("accept");
		return NULL;
	}
	// 2. 设置非阻塞
	int flag = fcntl(cfd, F_GETFL);
	flag |= O_NONBLOCK;
	fcntl(cfd, F_SETFL, flag);

	// 3. cfd添加到epoll树
	struct epoll_event ev;
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = cfd;
	int ret = epoll_ctl(info->epfd, EPOLL_CTL_ADD, cfd, &ev);
	if (ret == -1)
	{
		perror("epoll_ctl");
		return NULL;
	}
	printf("acceptClnt thread ID: %ld\n", info->tid);
	free(info);
	return NULL;
}

void* recvHttpRequest(void* arg)
{
	struct FdInfo* info = (struct FdInfo*)arg;
	printf("Start receiving data......\n");
	int len = 0, total = 0;
	char tmp[1024] = { 0 };
	char buf[4096] = { 0 };
	while ((len = recv(info->fd, tmp, sizeof tmp, 0)) > 0)
	{
		if (total + len < sizeof buf)
			memcpy(buf + total, tmp, len);
		total += len;
	}
	// 判断数据是否被接收完毕
	if (len == -1 && errno == EAGAIN)
	{
		// 解析请求行
		char* pt = strstr(buf, "\r\n");
		int reqLen = pt - buf;
		buf[reqLen] = '\0';
		parseRequestLine(buf, info->fd);
	}
	else if (len == 0)
	{
		// 客户端断开了连接
		epoll_ctl(info->epfd, EPOLL_CTL_DEL, info->fd, NULL);
		close(info->fd);
	}
	else
	{
		perror("recv");
	}
	printf("recvMsg thread ID: %ld\n", info->tid);
	close(info->fd);
	free(info);
	return NULL;
}

int parseRequestLine(const char* line, int cfd)
{
	// 解析请求行 get /xxx/1.jpg http/1.1
	char method[12];
	char path[1024];
	sscanf(line, "%[^ ] %[^ ]", method, path);
	printf("method: %s, path: %s\n", method, path);
	if (strcasecmp(method, "get") != 0)
		return -1;
	decodeMsg(path, path);
	// 处理客户端请求的静态资源(目录 or 文件)
	const char* file = NULL;
	if (strcmp(path, "/") == 0)
		file = "./";
	else
		file = path + 1;
	// 获取文件属性
	struct stat st;
	int ret = stat(file, &st);
	if (ret == -1)
	{
		// 文件不存在 -- 回复404
		sendHeadMsg(cfd, 404, "Not Found", getFileType(".html"), -1);
		sendFile("404.html", cfd);
		return 0;
	}
	// 判断文件类型
	if (S_ISDIR(st.st_mode))
	{
		// 把本地目录中的内容发送给客户端
		sendHeadMsg(cfd, 200, "OK", getFileType(".html"), -1);
		sendDir(file, cfd);
	}
	else
	{
		// 把文件的内容发送给客户端
		sendHeadMsg(cfd, 200, "OK", getFileType(file), st.st_size);
		sendFile(file, cfd);
	}

	return 0;
}

int sendFile(const char* fileName, int cfd)
{
	// 1. 打开文件
	int fd = open(fileName, O_RDONLY);
	assert(fd > 0);
	// 2. 把文件中的内容发送到客户端
#if 0
	while (1)
	{
		char buf[1024];
		int len = read(fd, buf, sizeof buf);
		if (len > 0)
		{
			send(cfd, buf, len, 0);
			usleep(10);	// 重要！！！
		}
		else if (len == 0)
		{
			break;
		}
		else
		{
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
	return 0;
}

int sendHeadMsg(int cfd, int status, const char* descr, const char* type, int length)
{
	// 状态行
	char buf[4096] = { 0 };
	sprintf(buf, "http/1.1 %d %s\r\n", status, descr);
	// 响应头
	sprintf(buf + strlen(buf), "content-type: %s\r\n", type);
	sprintf(buf + strlen(buf), "content-length: %d\r\n\r\n", length);

	int len = send(cfd, buf, strlen(buf), 0);
	if (len == -1)
	{
		perror("send");
		return -1;
	}

	return len;
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
int sendDir(const char* dirName, int cfd)
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
		send(cfd, buf, strlen(buf), 0);
		memset(buf, 0, sizeof(buf));
		free(namelist[i]);
	}
	sprintf(buf, "</table></body></html>");
	send(cfd, buf, strlen(buf), 0);
	free(namelist);
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