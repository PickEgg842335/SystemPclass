#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define QLEN 10

int echo(int fd){
    char buf[BUFSIZ];
    int  cc, count=0;

    cc = read(fd, buf, sizeof buf);
    if(cc < 0)
        printf("echo read\n");
    if(cc && write(fd, buf, cc) < 0)
        printf("echo write\n");
	if(cc>0)
		printf("echo to client: %s\n", buf);
    return cc;
}

int passivesock(const char *service, const char *transport, int qlen){
    struct servent  *pse;
    struct protoent *ppe;
    struct sockaddr_in sin;
    int     s, type;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;

    /* Map service name to port number */
    if(pse = getservbyname(service, transport))
        sin.sin_port = htons(1234);  //htons(pse->s_port);
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
        printf("can't create socket\n");

    /* Bind the socket */
    if(bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
        printf("can't bind to %s port\n");
    if(type == SOCK_STREAM && listen(s, qlen) < 0)
        printf("can't listen on %s port\n", service);
    return s;
}


int passiveTCP(const char *service, int qlen){
    return passivesock(service, "tcp", qlen);
}

int main(int argc, char *argv[ ]){
    char   *service = "echo";
    struct sockaddr_in fsin;
    int    msock;
    fd_set rfds;
    fd_set afds;
    int    alen;
    int    fd, nfds;
    msock = passiveTCP(service, QLEN);

    nfds = getdtablesize();
    FD_ZERO(&afds);
    FD_SET(msock, &afds);

    while (1) {
        memcpy(&rfds, &afds, sizeof(rfds));

      if(select(nfds, &rfds, (fd_set *)0, (fd_set *)0, (struct timeval *)0) < 0)
            printf("select:\n");
        if(FD_ISSET(msock, &rfds)){
    int ssock;

    alen = sizeof(fsin);
    ssock = accept(msock, (struct sockaddr *)&fsin, &alen);
    if(ssock < 0)
       printf("accept\n");
     FD_SET(ssock, &afds);
   }
   for(fd=0; fd < nfds; ++fd)
     if(fd != msock && FD_ISSET(fd, &rfds))
        if(echo(fd) == 0){
              (void) close(fd);
              FD_CLR(fd, &afds);
           }
    }
}

