#ifndef FUNCIONARIO_H
#define FUNCIONARIO_H

#define PORTA 9090

typedef struct {
	char nome[100];
	char sobrenome[100];
	char cidade[50];
	char estado[50];
	int idade;
} funcionario;

void cadastro_funcionario(funcionario cad);
void print_func(funcionario f);

#endif
