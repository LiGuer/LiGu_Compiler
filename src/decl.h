/*
Copyright (c) 1991,1992,1993,1994,1995,1996,1997,1998,1999,2000,2001,2002
by AT&T, Christopher W. Fraser, and David R. Hanson.    //[LCC]
Copyright (c) 2020 by LiGuer.
All Rights Reserved.

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
/******************************************************************************
*                    decl Analysis 声明__语法分析器
-------------------------------------------------------------------------------
*	[文法]:
	decl -> type id ;
	type -> type [ name ] | basic
******************************************************************************/
#ifndef DECL_H
#define DECL_H
#include "sym.h"
#include "lex.h"
#include "error.h"
#include "dag.h"
class Decl {
public:
	Lexical* lexical;
	Error* error;
	Decl(Lexical* _lexical, Error* _error) { lexical = _lexical; error = _error; }
	/*--------------------------------[ 声明分析 ]--------------------------------*/
	Tree* decl() {
		Type* ty = specifier();
		if (ty == NULL)return NULL;
		if (lexical->token != ID)error->error("Decl Not Find Identifier!");
		// Symbol
		Symbol id{ lexical->buffer.s, ty ,Sym_genLabel(1) };
		if(SymTablePos->insert(id) == false)error->error("ID had exsited!");
		// Tree
		Tree* p = new Tree;
		p->u.sym = SymTablePos->search(lexical->buffer.s);
		p->op = ty->type;
		lexical->getToken();
		error->expect(';');
		return p;
	}
	/*--------------------------------[ 标识符分析 ]--------------------------------*/
	Type* specifier() {
		static const int Byte = 8;
		Type* ty = new Type;
		while (true) {
			bool flag = 0;
			switch (lexical->token) {
			case BOOL: ty->size = 1; ty->type = INT; break;
			case CHAR: ty->size = 1 * Byte; ty->type = INT; break;
			case SHORT:ty->size = 2 * Byte; ty->type = INT; break;
			case INT: ty->size = 4 * Byte; ty->type = INT; break;
			case INT64: ty->size = 8 * Byte; ty->type = INT; break;
			case FLOAT: ty->size = 4 * Byte; ty->type = FLOAT; break;
			case FLOAT64: ty->size = 8 * Byte; ty->type = FLOAT; break;
			case STATIC: ty->sclass = STATIC; break;
			case CONST: ty->constant = true; break;
			case UNSIGNED: ty->sign = false; break;
			case VOID: ty->type = VOID; break;
			default: flag = 1; break;
			}
			if (flag)break;
			lexical->getToken();
		}
		if (ty->type == 0) { delete ty; ty = NULL; }
		return ty;
	}
};
#endif