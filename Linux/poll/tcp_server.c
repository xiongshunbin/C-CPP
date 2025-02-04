#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>

int main()
{
    // 1. 创建用于监听的套接字
    int lfd = socket(AF_INET, SOCK_STREAM, 0);
    if(lfd == -1)
    {
        perror("socket() error");
        exit(1);
    }

    // 2. 绑定本地IP和端口
    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(9999);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    int ret = bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(ret == -1)
    {
        perror("bind() error");
        exit(1);
    }

    // 3. 监听
    ret = listen(lfd, 100);
    if(ret == -1)
    {
        perror("listen() error");
        exit(1);
    }

    // 创建并初始化文件描述符集
    struct pollfd fds[1024] = {0};
    for(int i = 0; i < 1024; i ++)
    {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }
    fds[0].fd = lfd;

    int maxfd = 0;
    while(1)
    {
        // 委托内核检测
        ret = poll(fds, maxfd + 1, -1);
        if(ret == -1)
        {
            perror("poll() error");
            exit(1);
        }

        // 接受连接请求
        if(fds[0].revents & POLLIN)
        {
            struct sockaddr_in clnt_addr;
            socklen_t clnt_addr_len = sizeof(clnt_addr);
            int clnt_sock = accept(lfd, (struct sockaddr*)&clnt_addr, &clnt_addr_len);
            int i;
            for(i = 0; i < 1024; i ++)
            {
                if(fds[i].fd == -1)
                {
                    fds[i].fd = clnt_sock;
                    break;
                }
            }
            maxfd = i > maxfd ? i : maxfd;
        }

        // 通信
        for(int i = 1; i <= maxfd; i ++)
        {
            if(fds[i].revents & POLLIN)
            {
                char buf[128];
                int len = recv(fds[i].fd, buf, sizeof(buf), 0);
                if(len == -1)
                {
                    perror("recv() error");
                    exit(1);
                }
                else if(len == 0)
                {
                    printf("The other party has already disconnected......\n");
                    close(fds[i].fd);
                    fds[i].fd = -1;
                }
                else
                {
                    printf("The client say: %s\n", buf);
                    len = send(fds[i].fd, buf, strlen(buf) + 1, 0);
                }
            }
        }
    }

    close(lfd);

    return 0;
}