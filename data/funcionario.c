#include <stdlib.h>
#include "funcionario.h"

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

void preparar(int *sockfd, sockaddr_in *their_addr);
