#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *thread_function(void *arg);

int main()
{
	pthread_t pthread;
	int ret;
	int count = 8;

	ret = pthread_create(&pthread, NULL, thread_function, &count);
	if(ret != 0)
	{
		perror("pthread_create");
		exit(1);
	}

	pthread_join(pthread, NULL);
	printf("The thread is over, process is over too.\n");
	return 0;
}

void *thread_function(void *arg)
{
	int i;
	printf("Thread begins running\n");

	for(i = 0; i < *(int *)arg; i ++)
	{
		printf("Hello world\n");
		sleep(1);
	}
	return NULL;
}
