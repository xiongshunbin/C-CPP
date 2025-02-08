#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>

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

    // 3. 设置端口复用
    int optval = 1;
    int ret = setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
    if(ret == -1)
    {
        perror("setsockopt() error");
        exit(1);
    }

    ret = bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(ret == -1)
    {
        perror("bind() error");
        exit(1);
    }

    // 4. 监听
    ret = listen(lfd, 100);
    if(ret == -1)
    {
        perror("listen() error");
        exit(1);
    }

    // 创建epoll实例
    int epfd = epoll_create(1);
    if(epfd == -1)
    {
        perror("epoll_create() error");
        exit(1);
    }
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = lfd;
    ret = epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
    if(ret == -1)
    {
        perror("epoll_ctl() error");
        exit(1);
    }

    struct epoll_event evs[1024];
    int size = sizeof(evs) / sizeof(evs[0]);
    while(1)
    {
        int num = epoll_wait(epfd, evs, size, -1);
        printf("num = %d\n", num);
        for(int i = 0; i < num; i ++)
        {
            int fd = evs[i].data.fd;
            if(fd == lfd)
            {
                int cfd = accept(fd, NULL, NULL);
                ev.events = EPOLLIN;
                ev.data.fd = cfd;
                epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
            }
            else
            {
                char buf[1024];
                int len = recv(fd, buf, sizeof(buf), 0);
                if(len == -1)
                {
                    perror("recv() error");
                    exit(1);
                }
                else if(len == 0)
                {
                    printf("The client has disconnected......\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                    break;
                }
                printf("read buf = %s\n", buf);

                // 小写转大写
                for(int j = 0; j < len ; j ++)
                    buf[j] = toupper(buf[j]);

                printf("after buf = %s\n", buf);

                ret = send(fd, buf, strlen(buf) + 1, 0);
                if(ret == -1)
                {
                    perror("send() error");
                    exit(1);
                }
            }
        }
    }

    close(lfd);

    return 0;
}