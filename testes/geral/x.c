#include <stdlib.h>
#include <time.h>

int main() {
	int a, child = 0, parent = 0;
	while(1) {
		printf("Eu sou o pai!!\n");
		parent++;
		if((a = fork()) == -1) {
			break;
		} else if(a == 0) {
			printf("\nEu sou o filho!!\n");
			child++;
			sleep(2000);
		}
	}
	while(wait(NULL) > 0);
	printf("parent: %d - child: %d", parent, child);
}
