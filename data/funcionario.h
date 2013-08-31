#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#define PORTA 9090

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct hostent hostent;
typedef struct in_addr in_addr;

typedef struct {
	char nome[100];
	char sobrenome[100];
	char cidade[50];
	char estado[50];
	int idade;
} funcionario;

void cadastro_funcionario(funcionario cad);
void print_func(funcionario f);
funcionario *chartof(char *buf);
int preparar(int *sockfd, sockaddr_in *their_addr, hostent *he, int port);
int conectar(int sockfd, sockaddr_in *their_addr);
int enviar(int sockfd, char *package);

#endif
