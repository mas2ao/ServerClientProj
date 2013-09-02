#ifndef FIFO_H
#define FIFO_H

#include <sys/socket.h>

typedef struct {
	char package[400];
	int sock;
	short prox;
} fifo;

void inicia_fifo(char arq[]);
void adiciona(char arq[], char entra[], int sock);
fifo *remove_fifo(char arq[]);
void add_inicio(fifo aux, FILE *arq_ent, short pos);
void add_final(fifo aux, FILE *arq_ent);
void add_meio(fifo aux, short pos, FILE *arq_ent);
#endif
