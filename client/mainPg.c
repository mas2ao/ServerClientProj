#include <stdlib.h>
#include "../data/pessoa.h"

int main() {
	int sair = 0;
	while(sair == 0) {
		mostra_menu();
		switch(recebe_op()) {
			case 1:
				cadastrar();
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				sair = 1;
				break;
			default:
				printf("Opcao nao existente.\n");
				break;
		}
		printf("\n");
	}
	return 0;
}

void mostra_menu() {
	printf("    ------------------\n");
	printf("    - 1. Cadastrar   -\n");
	printf("    - 2. Consultar   -\n");
	printf("    - 3. Remover     -\n");
	printf("    ------------------\n");
	printf("    - 4. Sair        -\n");
	printf("    ------------------\n");
}

int recebe_op() {
	int op;
	char lin;
	printf("> Digite uma opcao: ");
	scanf("%d", &op);
	scanf("%c", &lin);

	return op;
}

void cadastrar() {
	pessoa cad;
	char lin;
	printf("\n> Nome: ");
	gets(cad.nome);
	printf("\n> Sobrenome: ");
	gets(cad.sobrenome);
	printf("\n> Cidade: ");
	gets(cad.nome);
	printf("\n> Estado: ");
	gets(cad.sobrenome);
	printf("\n> Idade: ");
	scanf("%d", &cad.idade);
	scanf("%c", &lin);

cadastrar_pessoa(cad);
}
