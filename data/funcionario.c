#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <errno.h>
#include <time.h>
#include "funcionario.h"

typedef struct hostent hostent;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct in_addr in_addr;

void cadastrar_funcionario(funcionario cad) {
	int server_number, ratio;
	int sock;
	sockaddr_in serv;
	hostent *he;
/*
	if(!(main = fopen("database/main.data", "r+b"))) {
		printf("Error opening the file!");
		exit(1);
	}

	fread(&server_number, sizeof(int), 1, main);

	ratio = server_number % 3;
*/	
	time_t tmp = time(0);
	server_number = tmp % 3;
	printf("%d\n", server_number);
	preparar(&sock, &serv, he);

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
	memset(&(their_addr->sin_zero), '\0', 8);
}
