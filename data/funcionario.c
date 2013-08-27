#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include "funcionario.h"

typedef struct hostent hostent;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct in_addr in_addr;

void cadastrar_funcionario(funcionario cad) {
	int sock;
	sockaddr_in inter;
	hostent *he;
/*
	if(!(main = fopen("database/main.data", "r+b"))) {
		printf("Error opening the file!");
		exit(1);
	}

	fread(&interer_number, sizeof(int), 1, main);

	ratio = interer_number % 3;
*/	
	preparar(&sock, &inter, he);
	conectar(sock, &inter, PORTA);

	close(sock);
}

void preparar(int *sockfd, sockaddr_in *their_addr, hostent *he) {
	if((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	if((he = gethostbyname("localhost")) == -1) {
		perror("gethostbyname");
		exit(1);
	}

	their_addr->sin_family = AF_INET;
	their_addr->sin_addr = *((in_addr *)he->h_addr);
	their_addr->sin_port = htons(PORTA);
	memset(&(their_addr->sin_zero), '\0', 8);
}

void conectar(int sockfd, sockaddr_in *their_addr) {
	if(connect(sockfd, (sockaddr *)their_addr, sizeof(sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}
}
