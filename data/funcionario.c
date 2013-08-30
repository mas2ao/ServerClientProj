#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <strings.h>
#include "funcionario.h"
#include "req.h"

typedef struct hostent hostent;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct in_addr in_addr;

int cadastrar_funcionario(funcionario cad) {
	int sock;
	cmd capsula;
	sockaddr_in inter;
	hostent *he;

	strcpy("add", &(capsula.command));
	if(!preparar(&sock, &inter, he)) return 0;
	if(!conectar(sock, &inter)) return 0;
	
	encapsular(&capsula, cad, 0);

	if(confirma(sock)) {
		if(!enviar(sock, capsula)) return 0;
	}

	close(sock);
	return 1;
}

int preparar(int *sockfd, sockaddr_in *their_addr, hostent *he) {
	if((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		return 0;
	}

	if((he = gethostbyname("localhost")) == -1) {
		perror("gethostbyname");
		return 0;
	}

	their_addr->sin_family = AF_INET;
	their_addr->sin_addr = *((in_addr *)he->h_addr);
	their_addr->sin_port = htons(PORTA);
	memset(&(their_addr->sin_zero), '\0', 8);
	
	return 1;
}

int conectar(int sockfd, sockaddr_in *their_addr) {
	if(connect(sockfd, (sockaddr *)their_addr, sizeof(sockaddr)) == -1) {
		perror("connect");
		return 0;
	}
	return 1;
}

void encapsular(cmd *capsula, funcionario func, int filter) {
	capsula->func = func;
	capsula->filter = filter;

	return capsula;
}

int confirma(int sockfd) {
	char buf[50];
	int num;
	
	if((num = recv(sockfd, buf, 49, 0)) == -1) {
		perror("recv");
		return 0;
	}
	if(strcmp(buf, "Connection Accepted")) {
		return 0;
	}

	return 1;
}

int enviar(int sockfd, cmd capsula) {
	if(send(sockfd, &capsula, sizeof(cmd), 0) == -1) {
		perror("send");
		return 0;
	}
	return 1;
}
