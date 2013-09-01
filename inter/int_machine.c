#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include <string.h>
#include <sys/shm.h>
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
		printf("%d -> sock_num\n", sock_recv);
		if(!fork()) {
			close(sock_list);
			while((bytes_recv = recv(sock_recv, package, sizeof(package), 0)) == -1) {
				perror("recv");
			}
			package[bytes_recv] = '\0';
			printf("Received connection from: %s\n", inet_ntoa(client_addr.sin_addr));	
			do_echo_command(sock_recv, package, &server_addr, he, client_addr);

			close(sock_recv);
			exit(0);
		}
		close(sock_recv);
	}

	return 0;
}

char *shm_config(char let) {	
	char *f;
	key_t *key = (key_t *) malloc(sizeof(key_t));
	int *shmid = (int *) malloc(sizeof(int));
	*key = ftok("int_machine.c", let);
	if((*shmid = shmget(*key, 400, 0644 | IPC_CREAT)) == -1) {
		perror("shmget");
		return NULL;
	}
	if((f = shmat(shmid, (void *)0, 0)) == (void *)-1) {
		perror("shmat");
		return NULL;
	}
	return f;
}

char *receber_pack(int sock) {
	char buf[400];
	int num_bytes;
	if((num_bytes = recv(sock, buf, sizeof(buf), 0)) == -1) {
		perror("recv");
		return NULL;
	} 
	buf[num_bytes] = '\0';
	return buf;
}

void do_echo_command(int sock_cli, char *buf, sockaddr_in *serv, hostent *he, sockaddr_in client) {
	char cmd[4], resp_char[400];
	int sock, resp, num_bytes, num;
	cmd[0] = buf[0];
	cmd[1] = buf[1];
	cmd[2] = buf[2];
	cmd[3] = '\0';
	if(!(num = strcmp(cmd, "add"))) {
		if(do_echo(&sock, buf, serv, he, 0)) {
			printf("Cadastrado com Sucesso!\n");
		} else {
			printf("Erro no Cadastramento!\n");
		}
		close(sock);
		return;
	}
	int sock1, sock2, opcao = 0, shmid;
	char *f = shm_config('R'), *g = shm_config('J'), *h = shm_config('x');

	if(!strcmp(cmd, "bus")) opcao = 1;

	if(!fork()) {
		if(!fork()) { //server1
			if(!do_echo(sock, buf, serv, he, 9990)) return;
			f = receber_pack(sock);
			if(strcmp(f, "end")) {
				enviar(sock_cli, f);
				if(opcao) {
					while(strcmp(f, "end")) {
						f = receber_pack(sock);
						enviar(sock_cli, f);
					}
				}
			}
			exit(0);
		} //server2
		if(!do_echo(sock, buf, serv, he, 9991)) return;
		g = receber_pack(sock);
		if(strcmp(g, "end")) {
			enviar(sock_cli, g);
			if(opcao) {
				while(strcmp(g, "end")) {
					g = receber_pack(sock);
					enviar(sock_cli, g);
				}
			}
		}
		while(wait(NULL) > 0);
		exit(0);
	} //server3
	if(!do_echo(sock, buf, serv, he, 9992)) return;
	h = receber_pack(sock);
	if(strcmp(h, "end")) {
		enviar(sock_cli, h);
		if(opcao) {
			while(strcmp(h, "end")) {
				h = receber_pack(sock);
				enviar(sock_cli, h);
			}
		}
	}
	while(wait(NULL) > 0);
}

void do_echo(int *sock, char *buf, sockaddr_in *serv, hostent *he, int port) {
	if(!port) {
		port = get_porta();
	}
	if(!preparar(sock, serv, he, port)) return 0;
	if(!conectar(*sock, serv)) return 0;
	if(!enviar(*sock, buf)) return 0;

	return 1;
}

int get_porta() {
	struct timeval tmp;
	gettimeofday(&tmp, NULL);
	int porta = (tmp.tv_usec % 3) + 9990;
	return porta;
}
