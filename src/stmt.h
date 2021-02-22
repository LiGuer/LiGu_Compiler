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
	stmts -> stmts stmt | ε
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
#ifndef STMT_H
#define STMT_H
#include "lex.h"
#include "error.h"
#include "expr.h"
#include "decl.h"
#include "dag.h"
class Stmt {
public:
	Lexical* lexical;
	Error* error;
	Expr* expr;
	Decl* decl;
	Stmt(Lexical* _lexical, Error* _error) { 
		lexical = _lexical; 
		error = _error;
		expr = new Expr(lexical, error);
		decl = new Decl(lexical, error);
	}
	/*--------------------------------[ stmts ]--------------------------------*/
	Tree* stmts(int looplabel) {
		Tree* tree = new Tree;
		Tree* root = tree;
		while (lexical->token == '{' || lexical->token < 18 || lexical->token == ID || (lexical->token >= 65 && lexical->token <= 75)) {
			Tree* p = stmt(looplabel);
			tree->kid[0] = p;
			tree->kid[1] = new Tree;
			tree = tree->kid[1];
		}
		return root;
	}
	/*--------------------------------[ 语句分析 ]--------------------------------*/
	Tree* stmt(int looplabel) {
		Tree* p = new Tree;
		switch (lexical->token) {
		case '{':	p = block(looplabel); break;
		case BREAK: {
			p->op = BREAK;
			p->u.v.i = looplabel + 1;
			lexical->getToken(); error->expect(';');
		}; break;
		case CONTINUE: {
			p->op = BREAK;
			p->u.v.i = looplabel;
			lexical->getToken(); error->expect(';');
		}; break;
		case FOR:	p = stmtFor(Sym_genLabel(2)); break;
		case GOTO:; break;
		case ID:	p = expr->expr(); error->expect(';'); break;
		case IF:	p = stmtIf(Sym_genLabel(1), looplabel); break;
		case SWITCH:p = stmtSwitch(Sym_genLabel(1), looplabel); break;
		case WHILE:	p = stmtWhile(Sym_genLabel(2)); break;
		case 1:case 2:case 3:case 4:case 5:case 6:case 7:case 8:case 9:
		case 10:case 11:case 12:case 13:case 14:case 15:case 16:case 17:
			p = decl->decl(); break;
		default:; break;
		}
		return p;
	}
	/*--------------------------------[ block "{ }" ]--------------------------------*/
	Tree* block(int looplabel) {
		// new Symbol Table
		SymbolTable* symtable = new SymbolTable;
		symtable->prev = SymTablePos;
		SymTablePos = symtable;
		//parse Analysis
		error->expect('{');
		Tree* root = stmts(looplabel);
		error->expect('}');
		// back Symbol Table
		SymTablePos = SymTablePos->prev;
		return root;
	}
	/*--------------------------------[ for语句 ]--------------------------------*/
	Tree* stmtFor(int newlable) {
		lexical->getToken();
		error->expect('(');
		Tree* p = new Tree;
		Tree* root = p;
		p->kid[0] = expr->expr();
		p->kid[1] = new Tree;
		p = p->kid[1];
		p->op = WHILE;
		p->u.v.i = newlable;
		error->expect(';');
		//
		p->kid[0] = expr->expr();
		error->expect(';');
		p->kid[1] = new Tree;
		p = p->kid[1];
		p->kid[1] = expr->expr();
		error->expect(')');
		p->kid[0] = stmt(newlable);
		return root;
	}
	/*--------------------------------[ if语句 ]--------------------------------*/
	Tree* stmtIf(int newlable, int looplabel) {
		lexical->getToken();
		Tree* p = new Tree;
		p->op = IF;
		p->u.v.i = newlable;
		error->expect('(');
		p->kid[0] = expr->expr();
		error->expect(')');
		p->kid[1] = stmt(looplabel);
		if (lexical->token == ELSE) {
			lexical->getToken();
		}
		return p;
	}
	/*--------------------------------[ switch语句 ]--------------------------------*/
	Tree* stmtSwitch(int newlable, int looplabel) {
		Tree* p = new Tree;
		return p;
	}
	/*--------------------------------[ while语句 ]--------------------------------*/
	Tree* stmtWhile(int newlable) {
		lexical->getToken();
		Tree* p = new Tree;
		p->op = WHILE;
		p->u.v.i = newlable;
		error->expect('(');
		p->kid[0] = expr->expr();
		error->expect(')');
		p->kid[1] = stmt(newlable);
		return p;
	}
};
#endif
