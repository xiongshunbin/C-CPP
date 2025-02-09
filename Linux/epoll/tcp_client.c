#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main()
{
    // 1. 创建用于通信的套接字
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket() error");
        exit(1);
    }

    // 2. 连接服务器
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9999);
    inet_pton(AF_INET, "172.30.62.109", &addr.sin_addr.s_addr);

    int ret = connect(fd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret == -1)
    {
        perror("connect() error");
        exit(1);
    }

    // 通信
    int num = 0;
    char buf[1024];
    while(num < 200)
    {
        sprintf(buf, "hello world, %d...\n", num ++);
        printf("%s\n", buf);
        ret = send(fd, buf, strlen(buf) + 1, 0);

        memset(buf, 0, sizeof(buf));

        int len = recv(fd, buf, sizeof(buf), 0);
        if(len == -1)
        {
            perror("recv() error");
            exit(1);
        }
        else if(len == 0)
        {
            printf("The other party has already disconnected......\n");
            break;
        }
        printf("recv buf: %s\n", buf);
        usleep(100000);
    }

    printf("---------- Game Over ----------\n");

    // 关闭套接字
    close(fd);

    return 0;
}