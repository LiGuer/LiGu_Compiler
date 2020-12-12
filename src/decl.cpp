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
#include "c.h"
/*--------------------------------[ 标识符分析 ]--------------------------------*/
Type* specifier() {
	Type* ty = new Type();
	while(true) {
		int flag = 0;
		switch (token) {
		case STATIC: ty->sclass = token; break;
		case CONST : ty->cons = token; break;
		case UNSIGNED: ty->sign = token; break;
		case VOID: ty->type = token; break;
		case BOOL: ty->size = 1; ty->type = INT; break;
		case CHAR: ty->size = 1 * 8; ty->type = INT; break;
		case SHORT:ty->size = 2 * 8; ty->type = INT; break;
		case INT:  ty->size = 4 * 8; ty->type = INT; break;
		case FLOAT: ty->size = 4 * 8; ty->type = FLOAT; break;
		case DOUBLE:ty->size = 8 * 8; ty->type = FLOAT; break;
		default: flag = 1; break;
		}
		if (flag)break;
		token = lex(lexBuffer);
	}
	if (ty->type == 0) { delete ty; ty = NULL; }
	return ty;
}
/*--------------------------------[ 声明分析 ]--------------------------------*/
Tree* decl() {
	Type* ty = specifier();
	if (ty == NULL)return NULL;
	if (token != ID)error("Decl Not Find Identifier!");
	// Symbol
	Symbol* id = new Symbol;
	id->name = lexBuffer.s;
	id->type = ty;
	Sym_insertGTable(id);
	// Tree
	Tree* p = new Tree;
	p->u.sym = id;
	p->op = ty->type;
	token = lex(lexBuffer);
	expect(';');
	return p;
}