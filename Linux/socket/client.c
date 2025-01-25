#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

int main()
{
    // 1. 创建用于通信的套接字
    int fd =socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
        perror("socket() error");
        return -1;
    }

    // 2. 连接服务器
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    inet_pton(AF_INET, "172.30.62.109", &saddr.sin_addr.s_addr);
    int ret = connect(fd, (struct sockaddr*)&saddr, sizeof(saddr));
    if(ret == -1)
    {
        perror("connect() error");
        return -1;
    }

    int number = 0;

    // 3. 通信
    while(1)
    {
        // 发送数据
        char buff[1024];
        sprintf(buff, "Hello world, %d...\n", number++);
        send(fd, buff, strlen(buff) + 1, 0);

        // 接收数据
        memset(buff, 0, sizeof(buff));
        int len = recv(fd, buff, sizeof(buff), 0);
        if(len > 0)
        {
            printf("server say: %s\n", buff);
        }
        else if(len == 0)
        {
            printf("The server has disconnecte...\n");
            break;
        }
        else
        {
            perror("recv() error");
            break;
        }
            
        sleep(1);
    }

    // 4. 关闭文件描述符
    close(fd);
    return 0;
}