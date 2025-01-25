#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX 50
int number = 0;

pthread_mutex_t mutex;

void* funcA_num(void *arg)
{
    for(int i = 0; i < MAX; i ++)
    {
        pthread_mutex_lock(&mutex);
        int cur = number;
        cur ++;
        usleep(10);
        number = cur;
        pthread_mutex_unlock(&mutex);
        printf("Thread A, id = %lu, number = %d\n", pthread_self(), number);
    }

    return NULL;
}

void* funcB_num(void *arg)
{
    for(int i = 0; i < MAX; i ++)
    {
        pthread_mutex_lock(&mutex);
        int cur = number;
        cur ++;
        number = cur;
        pthread_mutex_unlock(&mutex);
        printf("Thread B, id = %lu, number = %d\n", pthread_self(), number);
        usleep(5);
    }

    return NULL;
}

int main()
{
    pthread_t p1, p2;
    pthread_mutex_init(&mutex, NULL);

    pthread_create(&p1, NULL, funcA_num, NULL);
    pthread_create(&p2, NULL, funcB_num, NULL);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    pthread_mutex_destroy(&mutex);

    return 0;
}