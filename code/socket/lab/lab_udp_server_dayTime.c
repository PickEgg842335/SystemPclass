#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>

int passiveUDP(const char *service){
    return passivesock(service, "udp", 0);
}

u_short portbase = 0;
 
int passivesock(const char *service, const char *transport, int qlen){
    struct servent  *pse;
    struct protoent *ppe;
    struct sockaddr_in sin;
    int     s, type;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htons(INADDR_ANY);

    /* Map service name to port number */
    if(pse = getservbyname(service, transport))
      //  sin.sin_port = htons(ntohs((u_short)pse->s_port) + portbase);
		sin.sin_port = htons(1234);
    else if((sin.sin_port = htons((u_short)atoi(service))) == 0)
        printf("can't get \"%s\" service entry\n", service);

    /* Map protocol name to protocol number */
    if((ppe = getprotobyname(transport)) == 0)
        printf("can't get \"%s\" protocol entry\n", transport);

    /* Use protocol to choose a socket type */
    if(strcmp(transport, "udp") == 0)
        type = SOCK_DGRAM;
    else
        type = SOCK_STREAM;

    /* Allocate a socket */
    s = socket(PF_INET, type, 0);
    if(s < 0)
        printf("can't create socket: %s\n", strerror(errno));

    /* Bind the socket */
    if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        printf("can't bind to %s port: %s\n", service, strerror(errno));
    if(type == SOCK_STREAM && listen(s, qlen) < 0)
        printf("can't listen on %s port: %s\n", service, strerror(errno));
    return s;
}
/* main() - Iterative UDP server for TIME service */
int main(int argc, char *argv[ ]){
    struct sockaddr_in fsin;
    char *service = "time";
    char buf[1];
    int sock;
    time_t now;
    int alen;

    sock = passiveUDP(service);

 while (1){
     alen = sizeof(fsin);
     if(recvfrom(sock, buf, sizeof(buf), 0, (struct sockaddr *)&fsin, &alen) < 0)
        printf("recvfrom: %s\n", strerror(errno));
     time(&now);
     now = htonl((u_long)now);
     sendto(sock, (char *)&now, sizeof(now), 0, (struct sockaddr *)&fsin, sizeof(fsin));
 }
}
