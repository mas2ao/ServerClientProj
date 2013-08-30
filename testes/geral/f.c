#include <stdlib.h>

int main() {
	int b, c;
	printf("Parents pid: %d\n", getpid());
	b = fork();

	if (b > 0) {
		while(wait(NULL) > 0);
	} else {
		c = fork();
		if (c > 0) {
			while (wait(NULL) > 0);
		}
	}


	printf("The value of b is %d with pid = %d\n", b, getpid());

}
