#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER_PORT 6090
#define BUF_SIZE 50

int errno;

int main()
{
	int server_fd;
	int check, byte, len;
	struct sockaddr_in server_addr, client_addr;
	char buffer[BUF_SIZE];

	printf("Server : Creating socket...\n");
	server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(server_fd < 0)
	{
		printf("Server : Socket error(%d) : %s\n",errno,strerror(errno));
		perror("");
		return 1;
	}

	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);

	printf("Server : Binding socket...\n");
	check = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(check < 0)
	{
		printf("Server : Bind error(%d) : %s\n",errno,strerror(errno));
		return 1;
	}

	printf("Server : Receiving message...\n");
	byte = recvfrom(server_fd, buffer, BUF_SIZE-1, 0, (struct sockaddr *)&client_addr, &len);
	if(byte < 0)
	{
		printf("Server : Receive error(%d) : %s\n",errno,strerror(errno));
		return 1;
	}

	printf("Receive message : %s\n",buffer);

	close(server_fd);

	return 0;
}
