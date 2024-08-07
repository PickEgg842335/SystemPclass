/*  In alarm.c, the first function, ding, simulates an alarm clock.  */

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

static int alarm_fired = 0;

void ding(int sig)
{
    alarm_fired = 1;
}

/*  In main, we tell the child process to wait for five seconds
    before sending a SIGALRM signal to its parent.  */

int main()
{
    pid_t pid;

    printf("alarm application starting\n");
	printf("%d\n", getpid());
    pid = fork();
    switch(pid) {
    case -1:
      /* Failure */
      perror("fork failed");
      exit(1);
    case 0:
      /* child */
      sleep(5);
	  printf("child ppid %d\n", getppid());
	  printf("child pid %d\n", getpid());
      //kill(getppid(), SIGALRM);
      kill(getppid(), SIGINT);
		/*while(1){
			printf("Child ...\n");
			sleep(1);
		}*/
        exit(0);
    }

/*  The parent process arranges to catch SIGALRM with a call to signal
    and then waits for the inevitable.  */

    printf("waiting for alarm to go off\n");
	(void)signal(SIGINT, ding);
    pause();
	if(alarm_fired)
     printf("Ding!\n");
    printf("pid = %d,done\n", getpid());
	/*sleep(3);
	kill(pid, SIGINT);
    printf("done\n");*/
    exit(0);
}
