#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct sockaddr_in sockaddr_in;
typedef struct in_addr in_addr;
typedef struct hostent hostent;
typedef struct sockaddr sockaddr;
int main(int argc, char *argv[]) {
	hostent *he;
	sockaddr_in destaddr;
	char buff[100];
	int sockfd, numbytes;
	
	if((he = gethostbyname(argv[1])) == NULL) {
		perror("gethostbyname");
		exit(1);
	}

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	destaddr.sin_family = AF_INET;
	destaddr.sin_port = htons(9090);
	destaddr.sin_addr = *((in_addr *)he->h_addr);
	memset(&(destaddr.sin_zero), '\0', 8);

	if(connect(sockfd, (sockaddr *)&destaddr, sizeof(sockaddr)) == -1) {
		perror("connect");
		exit(1);
	}

	if((numbytes = recv(sockfd, buff, 99, 0)) == -1) {
		perror("recv");
		exit(1);
	}

	buff[numbytes] = '\0';
	printf("Received: %s", buff);
	close(sockfd);
	return 0;
}
