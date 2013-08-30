#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netdb.h>
#include "../data/funcionario.h"
#include "../data/req.h"

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;

int main() {
	int sock_list, sock_recv;
	int addr_in_size, bytes_recv;
	cmd *package = (cmd *) malloc(sizeof(cmd));
	sockaddr_in inter_addr, client_addr;

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
			while((bytes_recv = recv(sock_recv, package, sizeof(cmd), 0)) == -1) {
				perror("recv");
			}
			printf("%d bytes received from: %s\n", bytes_recv, inet_ntoa(client_addr.sin_addr));
			printf("Funcionario: %s %s\n", package->func.nome, package->func.sobrenome);
			printf("Mora em: %s-%s\n", package->func.cidade, package->func.estado);
			printf("Tem %d anos.\n\n", package->func.idade);

			close(sock_recv);
			exit(0);
		}
		close(sock_recv);
	}

	return 0;
}
