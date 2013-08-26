#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include "funcionario.h"

typedef struct hostent hostent;
typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

void cadastrar_funcionario(funcionario cad) {
	FILE *main;
	int server_number, ratio;

	if(!(main = fopen("database/main.data", "r+b"))) {
		printf("Error opening the file!");
		exit(1);
	}

	fread(&server_number, sizeof(int), 1, main);

	ratio = server_number % 3;

}

void preparar(int *sockfd, sockaddr_in *their_addr, hostent *he) {
	if((*sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	if((he = gethostbyname(
}
