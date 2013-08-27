#include "funcionario.h"
#ifndef REQ_H
#define REQ_H

typedef struct {
	char command[3];
	int filter;
	funcionario func;
} cmd;

#endif
