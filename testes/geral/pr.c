#include <stdlib.h>
#include <strings.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main() {
	key_t key;

	key = ftok("pr.c", 'G');
	int shmid, *x;

	shmid = shmget(key, 4, IPC_CREAT|IPC_EXCL|SHM_R|SHM_W);
	x = (int *)shmat(shmid, NULL, 0);
	*x = 10;
	shmdt(x);
	
}
