#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>

typedef struct sockaddr_in sockaddr_in;
int main() {
	int mysock, clisock;
	sockaddr_in myaddr, cliaddr;
	int size;

	if((mysock = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	myaddr.sin_family = AF_INET;
	myaddr.sin_port = htons(9090);
	myaddr.sin_addr.s_addr = INADDR_ANY;
	memset(&(myaddr.sin_zero), '\0', 8);

	if(bind(mysock, (struct sockaddr *)&myaddr, sizeof(myaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	if(listen(mysock, 10) == -1) {
		perror("listen");
		exit(1);
	}
	
	size = sizeof(struct sockaddr_in);
	while(1) {
		if((clisock = accept(mysock, (struct sockaddr *)&cliaddr, &size)) == -1) {
			perror("accept");
			continue;
		}
		printf("server: got connection from %s\n", inet_ntoa(cliaddr.sin_addr));
		if(!fork()) {
			close(mysock);
			if(send(clisock, "Hello World!\n", 14, 0) == -1) {
				perror("send");
			}
			close(clisock);
			break;
		}
	}
	while(wait(NULL) > 0);
	return 0;
}
