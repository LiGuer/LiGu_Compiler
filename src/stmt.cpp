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
*                    statement Analysis 控制语句__语法分析器
-------------------------------------------------------------------------------
*	[文法]:
	stmt -> expr ';'		
		  | block	= '{' stmts '}'	
	  	  | if '(' bool ')' stmt	
		  | if '(' bool ')' stmt else stmt
	  	  | while '(' bool ')' stmt
		  | do stmt while '(' expr ')'
		  | switch '(' expr ')' stmt
		  | for '(' [expr] ';' [expr] ';' [expr] ')' stmt
		  | break ';'
		  | continue ';'
		  | return [expr] ';'
******************************************************************************/
#include "c.h"
Tree* stmt();							//语句分析(主)
Tree* stmtIf();							//for语句
Tree* stmtFor();						//if语句
Tree* stmtSwitch();						//switch语句
Tree* stmtWhile();						//while语句
/*--------------------------------[ 语句分析(主) ]--------------------------------*/
Tree* stmt()	
{
	Tree* p = new Tree;
	switch (token) {
	case '{':	p = block(); break;
	case BREAK:; break;
	case CONTINUE:; break;
	case FOR:	p = stmtFor(); break;
	case ID:	p = expr(); expect(';'); break;
	case IF:	p = stmtIf(); break;
	case SWITCH:p = stmtSwitch(); break;
	case WHILE:	p = stmtWhile(); break;
	default:; break;
	}
	return p;
}
/*--------------------------------[ for语句 ]--------------------------------*/
Tree* stmtFor() {
	token = lex(lexBuffer);
	expect('(');
	Tree* p = new Tree;
	Tree* root = p;
	p->kid[0] = expr();
	p->kid[1] = new Tree;
	p = p->kid[1];
	p->op = WHILE;
	p->u.v.i = Sym_genLabel(2);
	expect(';');
	//
	p->kid[0] = expr();
	expect(';');
	p->kid[1] = new Tree;
	p = p->kid[1];
	p->kid[1] = expr();
	expect(')');
	p->kid[0] = stmt();
	return root;
}
/*--------------------------------[ if语句 ]--------------------------------*/
/*--------------------------------[ if语句 ]--------------------------------*/
Tree* stmtIf() {
	token = lex(lexBuffer);
	Tree* p = new Tree;
	p->op = IF;
	p->u.v.i=Sym_genLabel(1);
	expect('(');
	p->kid[0] = expr();
	expect(')');
	p->kid[1] = stmt();
	//if (t == ELSE) {stmtElse();}
	return p;
}
/*--------------------------------[ else语句 ]--------------------------------*/
Tree* stmtElse() {}
/*--------------------------------[ switch语句 ]--------------------------------*/
Tree* stmtSwitch() {
	Tree* p = new Tree;
	return p;
}
/*--------------------------------[ while语句 ]--------------------------------*/
Tree* stmtWhile() {
	token = lex(lexBuffer);
	Tree* p = new Tree;
	p->op = WHILE;
	p->u.v.i = Sym_genLabel(2);
	expect('(');
	p->kid[0] = expr();
	expect(')');
	p->kid[1] = stmt();
	return p;
}