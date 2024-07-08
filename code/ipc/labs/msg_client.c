/* The sender program is very similar to msg1.c. In the main set up, delete the
 msg_to_receive declaration and replace it with buffer[BUFSIZ], remove the message
 queue delete and make the following changes to the running loop.
 We now have a call to msgsnd to send the entered text to the queue. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/msg.h>
#include <pthread.h>

#define MAX_TEXT 512

void *thread_function(void *arg);

struct my_msg_st {
    long int my_msg_type;
	int pid;
    char some_text[MAX_TEXT];
};

int running = 1;
struct my_msg_st some_data;
struct my_msg_st some_data_r;
int msgid;
char buffer[BUFSIZ];
pid_t pid;
int main(){
    int res;
    pthread_t a_thread;

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }
	res = pthread_create(&a_thread, NULL, thread_function, NULL);	
    if (res != 0) {
        perror("Thread creation failed");
        exit(EXIT_FAILURE);
    }	
	while(running) {
		printf("Enter some text: ");
		fgets(buffer, BUFSIZ, stdin);
		some_data.my_msg_type = 10;
		some_data.pid = getpid();
		strcpy(some_data.some_text, buffer);

		if (msgsnd(msgid, (void *)&some_data, MAX_TEXT, 0) == -1) {
			fprintf(stderr, "msgsnd failed\n");
			exit(EXIT_FAILURE);
		}

		if (strncmp(buffer, "end", 3) == 0) {
			running = 0;
		}
	}
	res = pthread_join(a_thread, NULL);
    if (res != 0) {
        perror("Thread join failed");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}

void *thread_function(void *arg) {
	while(running){
		if (msgrcv(msgid, (void *)&some_data_r, BUFSIZ, (pid_t)getpid(), 0) == -1) {
			fprintf(stderr, "msgrcv failed with error: %d\n", errno);
			exit(EXIT_FAILURE);
		}else{
			printf("\n=> %s\n", some_data_r.my_msg_type, some_data_r.some_text);
		}
	}
    pthread_exit("");
}

