#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	int fd[2];
	if(pipe(fd) == -1)
		perror("pipe");
	pid = fork();
	if(pid == 0)	// child process: write to the pipe
	{
		char ch = '*';
		int n = 0;
		close(fd[0]);
		while(1)
		{
			write(fd[1], &ch, 1);
			printf("count = %d\n", ++ n);
		}
	}
	else if(pid > 0)	// parent process: wait until child process is over
	{
		waitpid(pid, NULL, 0);
	}
	return 0;
}