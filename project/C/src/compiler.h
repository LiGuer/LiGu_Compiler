#ifndef COMPILER_H
#define COMPILER_H
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include"lex.h"
#include"parse.h"
#include"error.h"
#include"IR.h"
class Compiler {
public:
	Error*   error;
	Lexical* lexical;
	Parse*   parse;
	IR* ir;

	Compiler(char* codePos){
		lexical = new Lexical(codePos);
		error   = new Error(lexical);
		parse   = new Parse(lexical, error);
		ir = new IR;
	}
	void compiler(char* output) {
		Tree* p = parse->parse();
		printf("\nGen IR now\n");
		ir->IRgen(p, output);
	}
};
#endif