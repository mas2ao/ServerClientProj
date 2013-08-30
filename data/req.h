#include "funcionario.h"
#ifndef REQ_H
#define REQ_H

#define PORTA 9090

typedef struct {
	char command[4];
	int filter;
	funcionario func;
} cmd;

#endif
