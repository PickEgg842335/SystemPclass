#include <stdio.h>
#include <stdlib.h>

int main(void){
	int pid,i;

	pid = fork();
	if (pid == 0) {
		for (i=0;i<10;i++) {
		    printf(" I am a child process I say %d\n",i);
		    sleep(1);
		  }
		exit(1);
	}else if (pid > 0) {
		for (i=0;i<10;i++) {
			printf(" I am a parent process I say %d\n",i);
		    sleep(1);
		}
	}else if (pid < 0)
		printf(" Sorry .....I can't fork my self\n");
}
