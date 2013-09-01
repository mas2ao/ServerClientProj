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

int send_funcionario(char cmd[], funcionario cad) {
	int sock;
	sockaddr_in inter;
	hostent *he;
	char package[400];

	ftochar(cmd, package, cad);

	if(!preparar(&sock, &inter, he, PORTA)) return 0;
	inter.sin_port = htons(PORTA);
	printf("Confirmation port: %d, %d", htons(PORTA), inter.sin_port);
	if(!conectar(sock, &inter)) return 0;
	printf("\nGot it!\n");
	if(!enviar(sock, package)) return 0;

	close(sock);
	return 1;
}

void ftochar(char *cmd, char *buf, funcionario func) {
	sprintf(buf, "%s|%s|%s|%s|%s|%d", cmd, func.nome, func.sobrenome, func.cidade, func.estado, func.idade);
}

void print_func(funcionario f) {
	printf("Funcionario: %s %s\n", f.nome, f.sobrenome);
	printf("De: %s-%s\n", f.cidade, f.estado);
	printf("%d anos\n\n", f.idade);
}

funcionario *chartof(char *buf) {
	int i = 0, flag = 0, j = 0;
	char idade[10];
	funcionario *func = (funcionario *) malloc(sizeof(funcionario));
	for(i=0; i<strlen(buf); i++) {
		if(buf[i] == '|') {
			j = 0;
			flag++;
		} else {
			switch(flag) {
				case 0:
					continue;
				case 1:
					func->nome[j] = buf[i];
					func->nome[j+1] = '\0';
					j++;
					break;
				case 2:
					func->sobrenome[j] = buf[i];
					func->sobrenome[j+1] = '\0';
					j++;
					break;
				case 3:
					func->cidade[j] = buf[i];
					func->cidade[j+1] = '\0';
					j++;
					break;
				case 4:
					func->estado[j] = buf[i];
					func->estado[j+1] = '\0';
					j++;
					break;
				case 5:
					idade[j] = buf[i];
					idade[j+1] = '\0';
					j++;
					break;
				default:
					i = 2000;
			}
		}
	}

	func->idade = atoi(idade);
	return func;
}

