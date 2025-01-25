#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t semp;
sem_t semc;
pthread_mutex_t mutex;

struct Node
{
    int number;
    struct Node* next;
};

struct Node* head = NULL;

void* producer(void* arg)
{
    while(1)
    {
        sem_wait(&semp);
        pthread_mutex_lock(&mutex);
        struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
        newNode->number = rand() % 1000;
        newNode->next = head;
        head = newNode;
        printf("Producer, id = %ld, number = %d\n", pthread_self(), newNode->number);
        pthread_mutex_unlock(&mutex);
        sem_post(&semc);
        sleep(rand() % 3);
    }
    return NULL;
}

void* consumer(void* arg)
{
    while(1)
    {
        sem_wait(&semc);
        pthread_mutex_lock(&mutex);
        struct Node* node = head;
        printf("Consumer, id = %ld, number = %d\n", pthread_self(), node->number);
        head = head->next;
        free(node);
        pthread_mutex_unlock(&mutex);
        sem_post(&semp);
        sleep(rand() % 3);
    }
    return NULL;
}

int main()
{
    sem_init(&semp, 0, 5);
    sem_init(&semc, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    pthread_t t1[5], t2[5];
    for(int i = 0; i < 5; i ++)
    {
        pthread_create(&t1[i], NULL, producer, NULL);
        pthread_create(&t2[i], NULL, consumer, NULL);
    }

    for(int i = 0; i < 5; i ++)
    {
        pthread_join(t1[i], NULL);
        pthread_join(t2[i], NULL);
    }

    sem_destroy(&semp);
    sem_destroy(&semc);
    pthread_mutex_destroy(&mutex);
    return 0;
}