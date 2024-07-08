#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#define MSG  "What time is it?\n"

int connectUDP(const char *host, const char *service)
{ return connectsock(host, service, "udp"); }

int connectsock(const char *host, const char *service, const char *transport){
    struct hostent      *phe;   /* pointer to host information entry    */
    struct servent      *pse;   /* pointer to service information entry */
    struct protoent     *ppe;   /* pointer to protocol information entry*/
    struct sockaddr_in sin;     /* an Internet endpoint address         */
    int s, type;                /* socket descriptor and socket type    */

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;

    /* Map service name to port number */
    if(pse = getservbyname(service, transport))
     //   sin.sin_port = pse->s_port;
		sin.sin_port = htons(1234);
    else if ((sin.sin_port = htons((unsigned short)atoi(service))) == 0)
        printf("can't get \"%s\" service entry\n", service);

    /* Map host name to IP address, allowing for dotted decimal */
/*    if(phe = gethostbyname(host))
        memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    else if ((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE)
        printf("can't get \"%s\" host entry\n", host);*/

    /* Map transport protocol name to protocol number */
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

    /* Connect the socket */
    if(connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
       printf("can't connect to %s.%s: %s\n", host, service, strerror(errno));
    return s;
}

int main(int argc, char *argv[ ]){
    char   *host = "localhost"; /* host to use if none supplied */
    char   *service = "time";   /* default service name   */
    time_t now;                 /* 32-bit integer to hold time  */ 
    int    s, n;                /* socket descriptor, read count */

    switch (argc) {
    case 1:
        host = "localhost";
        break;
    case 3:
        service = argv[2];
        /* FALL THROUGH */
    case 2:
        host = argv[1];
        break;
    default:
        fprintf(stderr, "usage: UDPtime [host [port]]\n");
        exit(1);
    }

    s = connectUDP(host, service);

    (void) write(s, MSG, strlen(MSG));

    /* Read the time */
    n = read(s, (char *)&now, sizeof(now));
    if(n < 0)
        printf("read failed: %s\n", strerror(errno));
    /* put in host byte order */
    now = ntohl((u_long)now);
    printf("%s", ctime(&now));
    exit(0);
}
