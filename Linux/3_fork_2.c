#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int count = 0; 

int main()
{
	pid_t pid;
	// int count = 0; 
	pid = fork();

	if(pid > 0)	// parent process
	{
		while(1)
		{
			printf("Hello world, count = %d\n", count ++);
			sleep(1);
		}
	}
	else if(pid == 0)	// child process
	{
		while(1)
		{
			printf("Good morning, count = %d\n", count ++);
			sleep(2);
		}
	}
	else
		perror("fork");
	return 0;
}