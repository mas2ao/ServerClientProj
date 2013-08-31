#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/time.h>
#include "../data/req.h"
#include "../data/funcionario.h"

int main() {
	char package[400];
	int sock_list, sock_recv, sock_send;
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
	printf("Listening on port -> %d\n", PORTA);

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

			if(!fork()) {
				doEcho(&sock_send, package, &server_addr, he);
				exit(0);
			}
			while(wait(NULL) > 0);
			close(sock_recv);
			exit(0);
		}
		close(sock_recv);
	}

	return 0;
}

void doEcho(int *sock, char *buf, sockaddr_in *serv, hostent *he) {
	if(!preparar(sock, serv, he, get_porta())) return 0;
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
