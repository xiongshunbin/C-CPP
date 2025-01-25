#include <stdio.h>
#include <pthread.h>

#define MAX 10
// pthread A and B count numbers one by one

int num = 0;

pthread_mutex_t mutex_A;
pthread_mutex_t mutex_B;

void *count_A(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex_A);
        if(num > MAX)
        {
            pthread_mutex_unlock(&mutex_B);
            pthread_mutex_unlock(&mutex_A);
            break;
        }
        num ++;
        printf("pthread A, id: %ld, count: %d\n", pthread_self(), num);
        pthread_mutex_unlock(&mutex_B);
    }
    
    return NULL;
}

void *count_B(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex_B);
        if(num > MAX)
        {
            pthread_mutex_unlock(&mutex_A);
            pthread_mutex_unlock(&mutex_B);
            break;
        }
        num ++;
        printf("pthread B, id: %ld, count: %d\n", pthread_self(), num);
        pthread_mutex_unlock(&mutex_A);
    }
    
    return NULL;
}

int main()
{
    pthread_t a, b;
    pthread_mutex_init(&mutex_A, NULL);
    pthread_mutex_init(&mutex_B, NULL);

    pthread_mutex_unlock(&mutex_B);    
    
    pthread_create(&a, NULL, count_A, NULL);
    pthread_create(&b, NULL, count_B, NULL);

    pthread_join(a, NULL);
    pthread_join(b, NULL);

    pthread_mutex_destroy(&mutex_A);
    pthread_mutex_destroy(&mutex_B);
    return 0;
}