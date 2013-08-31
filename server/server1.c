#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <mysql/mysql.h>
#include "../data/funcionario.h"
#include "servers.h"

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
MYSQL conexao;

int main() {
	int sock, sock_req, addr_in_size, bytes_recv;
	sockaddr_in req_addr, serv_addr;
	char package[400];
	funcionario *f;

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(serv_addr.sin_zero), '\0', 8);

	if(bind(sock, (sockaddr *)&serv_addr, sizeof(sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	if(listen(sock, BACKLOG_SERV) == -1) {
		perror("listen");
		exit(1);
	}

	while(1) {
		addr_in_size = sizeof(sockaddr_in);
		if((sock_req = accept(sock, (sockaddr *)&req_addr, &addr_in_size)) == -1) {
			perror("accept");
			continue;
		}

		if(!fork()) {
			close(sock);
			while((bytes_recv = recv(sock_req, package, sizeof(package), 0)) == -1) {
				perror("recv");
			}

			package[bytes_recv] = '\0';

			printf("%s\n", package);
			f = chartof(package);
			print_func(*f);

			close(sock_req);
			exit(0);
		}
		close(sock_req);
	}

	return 0;
}
