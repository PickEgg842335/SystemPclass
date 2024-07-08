#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define MY_PORT_ID   6090       /* a number > 5000 */

int errno;

main(void){
	int sockid, nread, addrlen;
	struct sockaddr_in my_addr, client_addr;
	char msg[50];
	
	printf("Server: creating socket\n");
	if ( (sockid = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		printf("Server: socket error: %d\n",errno); exit(0); 
	}
	
	printf("Server: binding my local socket\n");
	bzero((char *) &my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_addr.s_addr = htons(INADDR_ANY);
	my_addr.sin_port = htons(MY_PORT_ID);
	if ( (bind(sockid, (struct sockaddr *) &my_addr, sizeof(my_addr)) < 0) ){
		printf("Server: bind fail: %d\n",errno); exit(0); 
	}
	
	printf("Server: starting blocking message read\n");
	nread = recvfrom(sockid, msg, 30, 0, (struct sockaddr *) &client_addr, &addrlen);
	if(nread<0)
		printf("Server: retrun code from read is %d, error=%d, error reason\n",nread, errno, strerror(errno));
	else 
		printf("Server: message is: %s\n",msg);
	close(sockid);
}








