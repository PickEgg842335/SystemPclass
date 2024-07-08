/*  For our final example, server5.c, 
    we include the sys/time.h and sys/ioctl.h headers in place of signal.h
    in our last program and declare some extra variables to deal with select.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <unistd.h>

void checkin_fd(int fd_table[], int *cli_num, int fd){
	int i;
	
		for(i=0; i<5; i++)
			if(fd_table[i]==fd)
				return;
	
	if(*cli_num<5){	
		for(i=0; i<5; i++){
			if(fd_table[i]==0){
				fd_table[i]=fd;
				break;
			}
		}
		(*cli_num)++;
		return;
	}
}

void checkout_fd(int fd_table[], int *cli_num, int fd){
	int i;
	
		for(i=0; i<5; i++)
			if(fd_table[i]==fd){
				fd_table[i]=0;
				(*cli_num)--;
			}
}

void list_fd(int fd_table[], int cli_num){
	int i;

	printf("有 %d clients => ", cli_num);
	for(i=0; i<5; i++)
		if(fd_table[i]!=0)
			printf("%6d", fd_table[i]);
	printf("\n");
}

int main(void){
	int fd_table[5]={0}, cli_num=0,i;
	char buf[1024];
    int server_sockfd, client_sockfd;
    int server_len, client_len;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    int result;
    fd_set readfds, testfds;

/*  Create and name a socket for the server.  */

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(9734);
    server_len = sizeof(server_address);

    bind(server_sockfd, (struct sockaddr *)&server_address, server_len);

/*  Create a connection queue and initialize readfds to handle input from server_sockfd.  */

    listen(server_sockfd, 5);

    FD_ZERO(&readfds);
    FD_SET(server_sockfd, &readfds);

/*  Now wait for clients and requests.
    Since we have passed a null pointer as the timeout parameter, no timeout will occur.
    The program will exit and report an error if select returns a value of less than 1.  */

    while(1) {
        char ch;
        int fd;
        int nread;

        testfds = readfds;

        printf("server waiting\n");
        result = select(FD_SETSIZE, &testfds, (fd_set *)0, 
            (fd_set *)0, (struct timeval *) 0);

        if(result < 1) {
            perror("server5");
            exit(1);
        }

/*  Once we know we've got activity,
    we find which descriptor it's on by checking each in turn using FD_ISSET.  */

        for(fd = 0; fd < FD_SETSIZE; fd++) {
            if(FD_ISSET(fd,&testfds)) {

/*  If the activity is on server_sockfd, it must be a request for a new connection
    and we add the associated client_sockfd to the descriptor set.  */

                if(fd == server_sockfd) {
                    client_len = sizeof(client_address);
                    client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_address, &client_len);
                    FD_SET(client_sockfd, &readfds);
					checkin_fd(fd_table, &cli_num, client_sockfd);
                    printf("adding client on fd %d\n", client_sockfd);
                }

/*  If it isn't the server, it must be client activity.
    If close is received, the client has gone away and we remove it from the descriptor set.
    Otherwise, we 'serve' the client as in the previous examples.  */

                else {
                    ioctl(fd, FIONREAD, &nread);

                    if(nread == 0) {
                        close(fd);
                        FD_CLR(fd, &readfds);
                        printf("removing client on fd %d\n", fd);
                    }

                    else {
						read(fd, buf, nread+1);
						buf[nread]='\0';
						if(strncmp(buf, "end", 3)==0){
							FD_CLR(fd, &readfds);
							close(fd);
							checkout_fd(fd_table, &cli_num, fd);
						}else{
							// 轉送資料
							for(i=0; i<5; i++)
									if(fd_table[i]!=0 && fd_table[i]!=fd){
										printf("送出 pid:%d 的資料\n", buf);
										if (write(fd_table[i], buf, nread+1) == -1) {
											fprintf(stderr, "fd:%d msgsnd failed\n", fd_table[i]);
											exit(EXIT_FAILURE);
										}
									}
						}
                    }
                }
            }
        }
    }
}
