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
/******************************************************************************
*                    parse Analysis 语法分析器
*	[文法]:
	program -> block						{ return block.n;}
	block -> '{' decls stmts '}'			{ block.n = stmts.n; }
	decls -> decls decl | ε
	stmts -> stmts stmt	| ε				{ stmts.n = new Seq(stmts1.n, stmt.n);} { stmts.n = NULL;}
******************************************************************************/
#include "c.h"
int token;
Tree* parse();						//语法分析器(主)
Tree* block();						//代码块
Tree* decls();						//声明块
Tree* stmts();						//语句块
/*--------------------------------[ 语法分析器(主) ]--------------------------------*/
Tree* parse(){
	token = lex(lexBuffer);
	Tree* tree = new Tree;
	Tree* root = tree;
	while (token == '{') {
		Tree* p = block();
		tree->kid[0] = p;
		tree->kid[1] = new Tree;
		tree = tree->kid[1];
	}
	return root;
}
/*--------------------------------[ 代码块 ]--------------------------------*/
Tree* block() {
	expect('{');
	Tree* root = new Tree;
	root->kid[0] = decls();
	root->kid[1] = stmts();
	expect('}');
	return root;
}
/*--------------------------------[ 声明块 ]--------------------------------*/
Tree* decls() {
	Tree* tree = new Tree;
	Tree* root = tree;
	while (token < 18) {
		Tree* p = decl();
		tree->kid[0] = p;
		tree->kid[1] = new Tree;
		tree = tree->kid[1];
	}
	return root;
}
/*--------------------------------[ 语句块 ]--------------------------------*/
Tree* stmts() {
	Tree* tree = new Tree;
	Tree* root = tree;
	while (token != '}') {
		Tree* p = stmt();
		tree->kid[0] = p;
		tree->kid[1] = new Tree;
		tree = tree->kid[1];
	}
	return root;
}