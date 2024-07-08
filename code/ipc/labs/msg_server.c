/* Here's the receiver program. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include <sys/msg.h>
#define MAX_TEXT 512

struct my_msg_st {
    long int my_msg_type;
	pid_t pid;
    char some_text[BUFSIZ];
};

void checkin_pid(int pid_table[], int *cli_num, int pid){
	int i;
	
		for(i=0; i<5; i++)
			if(pid_table[i]==pid)
				return;
	
	if(*cli_num<5){	
		for(i=0; i<5; i++){
			if(pid_table[i]==0){
				pid_table[i]=pid;
				break;
			}
		}
		(*cli_num)++;
		return;
	}
}

void checkout_pid(int pid_table[], int *cli_num, int pid){
	int i;
	
		for(i=0; i<5; i++)
			if(pid_table[i]==pid){
				pid_table[i]=0;
				(*cli_num)--;
			}
}

void list_pid(int pid_table[], int cli_num){
	int i;

	printf("有 %d clients => ", cli_num);
	for(i=0; i<5; i++)
		if(pid_table[i]!=0)
			printf("%6d", pid_table[i]);
	printf("\n");
}

void msg_send(int msgid, int pid_table[], struct my_msg_st *some_data){
	int i;
	for(i=0; i<5; i++)
		if(pid_table[i]!=0 && pid_table[i]!=some_data->pid){
			some_data->my_msg_type=pid_table[i];
			printf("送出 pid:%d 的資料\n", some_data->pid);
			if (msgsnd(msgid, (void *)some_data, MAX_TEXT, 0) == -1) {
            	fprintf(stderr, "pid:%d msgsnd failed\n", some_data->pid);
            	exit(EXIT_FAILURE);
        	}
		}
}

int main(void){
	int pid_table[5]={0}, cli_num=0;
    int running = 1;
    int msgid;
    struct my_msg_st some_data;
    long int msg_to_receive = 0;

/* First, we set up the message queue. */

    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);

    if (msgid == -1) {
        fprintf(stderr, "msgget failed with error: %d\n", errno);
        exit(EXIT_FAILURE);
    }

/* Then the messages are retrieved from the queue, until an end message is encountered.
 Lastly, the message queue is deleted. */

    while(running) {
        if (msgrcv(msgid, (void *)&some_data, BUFSIZ, 10, 0) == -1) {
            fprintf(stderr, "msgrcv failed with error: %d\n", errno);
            exit(EXIT_FAILURE);
        }else{
			printf("pid: %d => %s", some_data.pid, some_data.some_text);

			checkin_pid(pid_table, &cli_num, some_data.pid);			// 有client送資料來
			list_pid(pid_table, cli_num);							// 顯示所有client
		
			if (strncmp(some_data.some_text, "end", 3) == 0){			// 有client離線
				checkout_pid(pid_table, &cli_num, some_data.pid);
				if(cli_num==0)		// 所有client離線
					running = 0;
			}else{
				msg_send(msgid, pid_table, &some_data);			// 送資料給其他client
			}
		}
    }

    if (msgctl(msgid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");
        exit(EXIT_FAILURE);
    }
	printf("全部離線\n");
    exit(EXIT_SUCCESS);
}
