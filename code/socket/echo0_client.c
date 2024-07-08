#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>

#define SERVER_PORT 6090
#define CLIENT_PORT 6089
#define SERVER_IP "127.0.0.1"
#define BUF_SIZE 50

int errno;

int main()
{
	int client_fd;
	int check, byte;
	struct sockaddr_in client_addr, server_addr;
	char buffer[BUF_SIZE];

	printf("Client : Creating socket...\n");
	client_fd = socket(AF_INET, SOCK_DGRAM, 0);
	if(client_fd < 0)
	{
		printf("Client : Socket error(%d) : ",errno);
		perror("");
		return 1;
	}

	bzero(&client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(CLIENT_PORT);
	client_addr.sin_addr.s_addr = htonl(INADDR_ANY);

	printf("Client : Binding socket...\n");
	check = bind(client_fd, (struct sockaddr *)&client_addr, sizeof(client_addr));
	if(check < 0)
	{
		printf("Client : Bind error(%d) : ",errno);
		perror("");
		return 1;	
	}
	
	printf("Client : Creating addr structure for server...\n");
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	printf("Client : Initialing message...\n");
	sprintf(buffer, "Hello world...");

	printf("Client : Sending message : %s\n",buffer);
	byte = sendto(client_fd, buffer, BUF_SIZE-1, 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
	if(byte < 0)
	{
		printf("Client : Sendto error(%d) : ",errno);
		perror("");
		return 1;			
	}

	close(client_fd);

	return 0;
}
