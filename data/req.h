#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#ifndef REQ_H
#define REQ_H

#define PORTA 9090

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct hostent hostent;
typedef struct in_addr in_addr;

int preparar(int *sockfd, sockaddr_in *their_addr, hostent *he, int port);
int conectar(int sockfd, sockaddr_in *their_addr);
int enviar(int sockfd, char *package);

#endif
