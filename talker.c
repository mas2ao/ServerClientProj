#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <netdb.h>

typedef struct sockaddr_in sockaddr_in;
typedef struct sockaddr sockaddr;
typedef struct hostent hostent;
typedef struct in_addr in_addr;
int main(int argc, char *argv[]) {
	hostent *he;
	sockaddr_in destaddr;
	int sockfd, numbytes;
	
	if((he = gethostbyname(argv[1])) == NULL) {
		perror("gethostbyname");
		exit(1);
	}

	if((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	destaddr.sin_family = AF_INET;
	destaddr.sin_port = htons(9090);
	destaddr.sin_addr = *((in_addr *)he->h_addr);
	memset(&(destaddr.sin_zero), '\0', 8);

	if((numbytes = sendto(sockfd, argv[2], strlen(argv[2]), 0, (sockaddr *)&destaddr, sizeof(sockaddr))) == -1) {
		perror("sendto");
		exit(1);
	}

	printf("sent %d bytes from %s", numbytes, inet_ntoa(destaddr.sin_addr));

}
