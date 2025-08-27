#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "common.h"

int recvMsg(int fd, char** buf)
{
    // 读包头
    int netlen = 0;
    // 将数据包长度读入 netlen 中
    int ret = readn(fd, (char*)&netlen, sizeof(int));
    // 将数据包由大端字节序转换为小端字节序
    int len = ntohl(netlen);
    printf("数据包长度: %d\n", len);
    // 读数据
    char* tmp = (char *)malloc(len + 1);
    ret = readn(fd, tmp, len);
    tmp[len] = '\0';
    *buf = tmp;
    return ret;
}

int main()
{
    // 1.创建用于通信的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket");
        exit(1);
    }

    // 2.连接服务器
    struct sockaddr_in addr_server;
    addr_server.sin_family = AF_INET;
    inet_pton(AF_INET, "127.0.0.1", &addr_server.sin_addr.s_addr);
    addr_server.sin_port = htons(8989);
    int ret = connect(fd, (struct sockaddr*)&addr_server, sizeof(addr_server));
    if(ret == -1)
    {
        perror("connect");
        exit(1);
    }

    // 通信
    while(1)
    {
        char* recvBuf = NULL;
        int ret = recvMsg(fd, &recvBuf);
        if(ret == -1)
        {
            break;
        }
        else if(ret == 0)
        {
            printf("服务器已经断开了连接......\n");
            break;
        }
        else
        {
            printf("收到的数据：%s", recvBuf);
        }
        printf("\n\n==============================\n\n");
        sleep(rand() % 3);
    }

    // 释放资源
    close(fd);

    return 0;
}