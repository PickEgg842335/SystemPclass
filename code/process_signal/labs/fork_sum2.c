#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(void){

	pid_t pid_1, pid_2;
	int i, sum;
	int result;

	printf("fork start...\n");
	pid_1=fork();
//	pid_2=fork();

	switch(pid_1){
		case -1:
			perror("fork failed..\n");
			exit(1);
		case pid_1:
			for(sum=0, i=1; i<=10; i++)
				sum+=i;
			exit(sum);
		default:
			
	}

	wait(&result);
	if(WIFEXITED(result))
		printf("1+2+...+10= %d\n", WEXITSTATUS(result));
	else
		printf("Child terminated abnormally\n");

	return 0;
}


