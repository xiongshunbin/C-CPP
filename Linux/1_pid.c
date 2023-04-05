#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
	pid_t pid;
	while(1)
	{
		printf("pid = %d\n", getpid());
		printf("ppid = %d\n", getppid());
		printf("Hello world\n");
		sleep(1);
	}
	return 0;
}