#include <sys/socket.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>

typedef struct sockaddr_in sockaddr_in;
int main() {
	int mysock, clisock;
	char buff[100];
	sockaddr_in myaddr, cliaddr;
	int size;

	if((mysock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
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
			sprintf(buff, "Hello World from process %d!\n", getpid());
			if(send(clisock, buff, strlen(buff), 0) == -1) {
				perror("send");
				exit(1);
			}
			close(clisock);
			exit(0);
		}
	}
	while(wait(NULL) > 0);
	return 0;
}
