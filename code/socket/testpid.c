#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int i, pid, x=0, count=0;
	
	pid = fork();

	printf("Forking the pid: %d\n", pid);

	for(i=0; i<5; i++)
		printf("%d pid=%d\n", i, getpid());

	if(pid){	// parent process
		printf("Hello %d, my parent pid=%d\n", getpid(), getppid());
		count+=1;
		printf("count+1=%d\n", count);
		wait(NULL);
	}else{		// child process
		sleep(5);
		count+=2;
		printf("count+2=%d\n", count);
		printf("hi %d, my parent pid=%d\n", getpid(), getppid());
	}

	return 0;
}

