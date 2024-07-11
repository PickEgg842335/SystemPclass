#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    char *message;
    int n;
    int exit_code;
	int sum = 0;

    printf("fork program starting\n");
    pid = fork();
    switch(pid) 
    {
    case -1:
        exit(1);
    case 0:	
        message = "This is the child";
        for(int i = 1; i <= 10; i++)
        {
            sum += i;
        }
        exit_code = sum;
        break;
    }

/*  This section of the program waits for the child process to finish.  */

    if(pid) {
        int result;
        pid_t child_pid;

        child_pid = wait(&result);

        printf("Child has finished: PID = %d\n", child_pid);
        if(WIFEXITED(result))
            printf("Child cal sum is %d\n", WEXITSTATUS(result));
        else
            printf("Child terminated abnormally\n");
    }
    exit (exit_code);
}
