#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../data/funcionario.h"

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

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

int main() {
	char package[400];
	int sock_list, sock_recv;
	int addr_in_size, bytes_recv;
	sockaddr_in inter_addr, client_addr;
	funcionario *f, func;

	if((sock_list = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}
	inter_addr.sin_family = AF_INET;
	inter_addr.sin_port = htons(PORTA);
	inter_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(inter_addr.sin_zero), '\0', 8);

	if(bind(sock_list, (sockaddr *)&inter_addr, sizeof(sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}
	
	if(listen(sock_list, 10) == -1) {
		perror("listen");
		exit(1);
	}

	while(1) {
		addr_in_size = sizeof(sockaddr_in);
		if((sock_recv = accept(sock_list, (sockaddr *)&client_addr, &addr_in_size)) == -1) {
			perror("accept");
			continue;
		}

		if(!fork()) {
			close(sock_list);
			while((bytes_recv = recv(sock_recv, package, sizeof(package), 0)) == -1) {
				perror("recv");
			}
		
			printf("%s\n", package);
			f = chartof(package);
			print_func(*f);

			close(sock_recv);
			exit(0);
		}
		close(sock_recv);
	}

	return 0;
}

