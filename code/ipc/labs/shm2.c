/* The second program is the producer and allows us to enter data for consumers.
 It's very similar to shm1.c and looks like this. */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <sys/shm.h>

#include "shm_com.h"

int main()
{
    int running = 1;
    void *shared_memory = (void *)0;
    struct shared_use_st *shared_stuff;
    char buffer[BUFSIZ];
    int shmid;

    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);

    if (shmid == -1) {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }

    shared_memory = shmat(shmid, (void *)0, 0);
    if (shared_memory == (void *)-1) {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }

    printf("Memory attached at %X\n", (int)shared_memory);

    shared_stuff = (struct shared_use_st *)shared_memory;
    while(running) {
        if (shared_stuff->written_by_you==4) {		// shm2 write
        	printf("shm1: Enter some text: ");
        	fgets(buffer, BUFSIZ, stdin);
			strncpy(shared_stuff->some_text, buffer, TEXT_SZ);
            shared_stuff->written_by_you = 3;
            sleep( rand() % 4 ); /* make the other process wait for us ! */
            if (strncmp(shared_stuff->some_text, "end", 3) == 0) {
                running = 0;
            }
        }
		if(shared_stuff->written_by_you==1) {		// shm2 read
			printf("shm2 writes: %s\n", shared_stuff->some_text);
			shared_stuff->written_by_you=4;
        	if (strncmp(shared_stuff->some_text, "end", 3) == 0) {
            	running = 0;
        	}
		}
    }

    if (shmdt(shared_memory) == -1) {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}
