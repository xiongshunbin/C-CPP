/*
	parent process: write pipe
	child process: read pipe
*/
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
int main()
{
	int fd[2];
	int pid;
	if(pipe(fd) == -1)
		perror("pipe");
	pid = fork();
	if(pid > 0)	// parent process
	{
		close(fd[0]);
		sleep(5);
		write(fd[1], "ab", 2);
		while(1);
	}
	else if(pid == 0)
 	{
		char ch[2];
		printf("Child process is waiting for data: \n");
		close(fd[1]);
		read(fd[0], ch, 2);
		printf("Read from pipe: %s", ch);
	}
	return 0;
}
