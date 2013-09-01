#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include "../data/req.h"
#include "../data/funcionario.h"

int main() {
	char package[400];
	int sock_list, sock_recv;
	int addr_in_size, bytes_recv;
	hostent *he;
	sockaddr_in inter_addr, client_addr, server_addr;
	funcionario *f;

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
			package[bytes_recv] = '\0';

			do_echo_command(package, &server_addr, he);
			
			close(sock_recv);
			exit(0);
		}
		close(sock_recv);
	}

	return 0;
}

void do_echo_command(char *buf, sockaddr_in *serv, hostent *he) {
	char cmd[4], resp_char[400];
	int sock, resp, num_bytes, num;
	cmd[0] = buf[0];
	cmd[1] = buf[1];
	cmd[2] = buf[2];
	cmd[3] = '\0';
	if(!(num = strcmp(cmd, "add"))) {
		if(repassa_com(&sock, buf, serv, he, 0)) {
			printf("Cadastrado com Sucesso!");
		} else {
			printf("Erro no Cadastramento!");
		}
		return;
	}
	printf("%d -> cmp\n", num);
	int sock1, sock2, opcao = 0;
	if(!strcmp(cmd, "bus")) opcao = 1;

	if(!fork()) {
		if(!fork()) { //server1
			if(repassa_com(&sock, buf, serv, he, 9990)) {
				if(opcao) {

				} else {
					printf("Removido com Sucesso!");
				}
			}
			exit(0);
		} //server2
		if(repassa_com(&sock1, buf, serv, he, 9991)) {
			if(opcao) {

			} else {
				printf("Removido com Sucesso!");
			}
		}
		while(wait(NULL) > 0);
		exit(0);
	} //server3
	if(repassa_com(&sock2, buf, serv, he, 9992)) {
		if(opcao) {

		} else {
			printf("Removido com Sucesso!");
		}
	}
	while(wait(NULL) > 0);
}

char *receber(int sock) {
	char *buf = (char *) malloc(sizeof(char)*400);
	int num_bytes;
	if((num_bytes = recv(sock, buf, sizeof(buf), 0)) == -1) {
		perror("recv");
		return NULL;
	} 
	buf[num_bytes] = '\0';
	return buf;
}

int repassa_com(int *sock, char *buf, sockaddr_in *serv, hostent *he, int port) {
	int resp;
	if(!do_echo(sock, buf, serv, he, port)) return 0;
	while(recv(sock, &resp, sizeof(resp), 0) == -1) {
		perror("recv");
		return 0; 
	}
	return resp;
}

void do_echo(int *sock, char *buf, sockaddr_in *serv, hostent *he, int port) {
	if(!port) {
		port = get_porta();
	}
	if(!preparar(sock, serv, he, port)) return 0;
	if(!conectar(*sock, serv)) return 0;
	if(!enviar(*sock, buf)) return 0;

	close(*sock);
}

int get_porta() {
	struct timeval tmp;
	gettimeofday(&tmp, NULL);
	int porta = (tmp.tv_usec % 3) + 9990;
	return porta;
}
