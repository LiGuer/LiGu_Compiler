#ifndef CALCULATOR_TOKEN_H
#define CALCULATOR_TOKEN_H
// opject
enum { NUM, REAL, VAR };

// operator
enum { NEG, ADD, SUB, MUL, DIV, POWER, FUCTION};

int prio[256];

void prioinit() {
    prio['+'] = 4; prio['-'] = 4;
    prio['*'] = 5; prio['/'] = 5;
}


struct Value {
	union {
			long long   i;
			long double r;
	} v;

	char* name;
};

#endif
