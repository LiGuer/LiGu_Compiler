#ifndef DAG_H
#define DAG_H
#include<stdlib.h>
#include "sym.h"
#include "lex.h"
// �����﷨��
class Tree {
public:
	int op = 0;
	union {
		Value v;
		Symbol* sym;
	}u;
	Tree* kid[2] = { NULL,NULL };
};
#endif