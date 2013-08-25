#ifndef PESSOA_H
#define PESSOA_H



typedef struct {
	char nome[100];
	char sobrenome[100];
	char cidade[50];
	char estado[50];
	int idade;
} pessoa;

void cadastro_pessoa(pessoa cad);

#endif
