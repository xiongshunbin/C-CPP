#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

typedef struct SockInfo{
    int fd;
    int epfd;
}SockInfo;

void* acceptConn(void* arg)
{
    printf("acceptConn tid: %ld\n", pthread_self());
    SockInfo* info = (SockInfo*)arg;
    int cfd = accept(info->fd, NULL, NULL);
    // 设置非阻塞属性
    int flag = fcntl(cfd, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(cfd, F_SETFL, flag);
    struct epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = cfd;
    epoll_ctl(info->epfd, EPOLL_CTL_ADD, cfd, &ev);

    free(info);

    return NULL;
}

void* communication(void* arg)
{
    printf("communication tid: %ld\n", pthread_self());
    SockInfo* info = (SockInfo*)arg;
    char buf[5];
    char temp[1024];
    bzero(temp, sizeof(temp));
    while(1)
    {
        int len = recv(info->fd, buf, sizeof(buf), 0);
        if(len == -1)
        {
            if(errno == EAGAIN)
            {
                printf("The data has been received completely......\n");
                int ret = send(info->fd, temp, strlen(temp) + 1, 0);
                if(ret == -1)
                    perror("send() error");
            }
            else
                perror("recv() error");
            break;
        }
        else if(len == 0)
        {
            printf("The client has disconnected......\n");
            epoll_ctl(info->epfd, EPOLL_CTL_DEL, info->fd, NULL);
            close(info->fd);
            break;
        }

        // 小写转大写
        for(int j = 0; j < len ; j ++)
            buf[j] = toupper(buf[j]);
        strncat(temp + strlen(temp), buf, len);

        write(STDOUT_FILENO, buf, len);
    }

    free(info);

    return NULL;
}

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
    ev.events = EPOLLIN | EPOLLET;
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
        pthread_t tid;
        for(int i = 0; i < num; i ++)
        {
            int fd = evs[i].data.fd;
            SockInfo* info = (SockInfo*)malloc(sizeof(SockInfo));
            info->fd = fd;
            info->epfd = epfd;
            if(fd == lfd)
                pthread_create(&tid, NULL, acceptConn, info);
            else
                pthread_create(&tid, NULL, communication, info);
            pthread_detach(tid);
        }
    }

    close(lfd);

    return 0;
}