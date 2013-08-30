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

typedef struct hostent hostent;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct in_addr in_addr;

int cadastrar_funcionario(funcionario cad) {
	int sock;
	sockaddr_in inter;
	hostent *he;
	char package[400];
	
	ftochar("add", package, cad);

	if(!preparar(&sock, &inter, he)) return 0;
	if(!conectar(sock, &inter)) return 0;
	
	if(!enviar(sock, package)) return 0;
	
	close(sock);
	return 1;
}

void ftochar(char *cmd, char *buf, funcionario func) {
	sprintf(buf, "%s|%s|%s|%s|%s|%d", cmd, func.nome, func.sobrenome, func.cidade, func.estado, func.idade);
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

int enviar(int sockfd, char *package) {
	if(send(sockfd, package, strlen(package), 0) == -1) {
		perror("send");
		return 0;
	}
	return 1;
}

void print_func(funcionario f) {
	printf("Funcionario: %s %s\n", f.nome, f.sobrenome);
	printf("De: %s-%s\n", f.cidade, f.estado);
	printf("%d anos\n\n", f.idade);
}
