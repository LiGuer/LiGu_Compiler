/*
Copyright (c) 1991,1992,1993,1994,1995,1996,1997,1998,1999,2000,2001,2002 by AT&T, Christopher W. Fraser, and David R. Hanson.    //[LCC]
Copyright (c) 2020 by LiGuer. All Rights Reserved.
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
	http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
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