#include <stdlib.h>
#include <sys/time.h>

int main() {
	struct timeval t;
	gettimeofday(&t, NULL);
	int a = 10;
	a = t.tv_usec/a;
	printf("time: %d\n", a);
}
