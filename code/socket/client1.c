/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <unistd.h>

int main(void){
    int sockfd;
    int len;
    struct sockaddr_un address;
    int result;
    char ch = 'X';

/*  Create a socket for the client.  */

    sockfd = socket(AF_UNIX, SOCK_STREAM, 0);

/*  Name the socket, as agreed with the server.  */

    address.sun_family = AF_UNIX;
    strcpy(address.sun_path, "server_socket");
    len = sizeof(address);

/*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("oops: client1");
        exit(1);
    }

/*  We can now read/write via sockfd.  */
	while(1){
		write(sockfd, &ch, 1);
		read(sockfd, &ch, 1);
		printf("char from server = %c\n", ch);
		
		if(ch=='Q' || ch=='q'){
			close(sockfd);
			exit(0);
		}
	}
}
