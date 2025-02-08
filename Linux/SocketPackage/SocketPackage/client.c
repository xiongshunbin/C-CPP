#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include "socket.h"

int main()
{
	// 1. 创建通信的套接字
	int fd = createSocket();
	if (fd == -1)
		return -1;

	// 2. 连接服务器
	int ret = connectToServer(fd, "172.30.62.109", 10000);
	if (ret == -1)
		return -1;

	// 3. 通信
	int fd1 = open("english.txt", O_RDONLY);
	if (fd1 == -1)
	{
		perror("open() error");
		closeSocket(fd);
		return -1;
	}
	int length = 0;
	char buf[1000];
	while ((length = read(fd1, buf, rand() % 1000)) > 0)
	{
		// 发送数据
		sendMsg(fd, buf, length);

		memset(buf, 0, sizeof(buf));
		usleep(300);
	}

	sleep(10);
	// 4. 关闭套接字
	closeSocket(fd);

	return 0;
}