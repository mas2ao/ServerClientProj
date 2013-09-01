#include <stdlib.h>
#include <time.h>
#include <string.h>

int main() {
	char a[] = "add";
	char b[3];
	b[0] = 'a';
	b[1] = 'd';
	b[2] = 'd';
	if(!strcmp(b, "add")) printf("SHIT %d\n", sizeof(b));
	if(!strcmp(a, "add")) printf("%d\n",sizeof(a));
}
