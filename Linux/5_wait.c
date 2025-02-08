#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// run model: ./a.out 10 5 15 (three child process, after10, 5, 15seconds, they are over)
int main(int argc, char *argv[])
{
    pid_t child_pid;
    int numDead;
    int n;

    for(int i = 1; i < argc; i ++)
    {
        switch(fork())
        {
            case -1:
                perror("fork()");
                exit(0);
            case 0:
                printf("Child %d started with PID = %d, sleeping %s seconds\n", i, getpid(), argv[i]);
                sleep(atoi(argv[i]));
                exit(0);
            default:
                break;
        }
    }
    numDead = 0;
    while(1)
    {
        child_pid = wait(NULL);
        if(child_pid == -1)
        {
            printf("No more children, Byebye!\n");
            exit(0);
        }
        numDead ++;
        printf("wait() returned child PID: %d(numDead = %d)\n", child_pid, numDead);
    }
	return 0;
}
