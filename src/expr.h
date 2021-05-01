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
*                    expression Analysis 表达式__语法分析器
-------------------------------------------------------------------------------
*	[文法]:
	expr -> assign_expr { ',' expr }
	assign_expr -> unary_expr assign_operator(=,+=,-=,*=,/=,%=,<<=,>>=,&=.^=,|=) assign_expr
				| binary_expr
	binary_expr -> binary_expr { binary_operator(||,&&,|,^,&,==,!=,<,>,<=,>=,<<,>>,+,-,*,/,%) unary_expr }
				| unary_expr
	unary_expr -> unary_operator(++,--,&,*,+,-,~,!) unary_expr
				| '(' type_name ')' unary_expr
				| primary_expr
	primary_expr -> id
				| const
				| string
				| '(' binary_expr ')'
-------------------------------------------------------------------------------
*	[性质]:
	[1]消除左递归:
		(立即左递归消除)
		E -> E a1 | E a1 | b1 | b2		(左递归) (a,b:不以E开头的终结符/非终结符)
		E -> b1 E'| b2 E'	E'-> a1 E'| a2 E'|ε	(ε:空集)
	[2]提取左因子
		E -> a b1 | a b2
		E -> a E'	E'-> b1 | b2 
	[3] 二义性
		不能出现 E -> E + E, 因为其存在不止一棵推导树
-------------------------------------------------------------------------------
*	[binary_expr(k)]   k : 运算符优先级
		expr1 -> expr1 op(>,<) expr2 | expr2
		expr2 -> expr2 op(+,-) expr3 | expr3
		expr3 -> expr3 op(*,/) prim  | prim
		prim  -> id | const | '(' expr ')'
	(消除左递归)
		expr1 -> expr2 expr1'     expr1' -> op(>,<) expr2 expr1' | ε ...
		expr3 -> prim  expr3'     expr3' -> op(*,/) prim  expr3' | ε
	(所以)
		expr1 -> expr2 { op(>,<) expr2 }	...	//expr1->expr2{op1 expr1}不行，出现左结合顺序错误(?)
		expr3 -> prim  { op(*,/) expr2 }
	**低优先级expr可直接调用高优先级expr，但高优先级expr不允许调用低优先级expr。
	(所以,优化)
		expr(n)-> prim [{ op(k) expr(k+1) }{ op(k-1) expr(k) }...{ op(n) expr(n+1) }]
******************************************************************************/
#ifndef EXPR_H
#define EXPR_H
#include "lex.h"
#include "error.h"
#include "dag.h"
int prio[] = {									//操作符优先级
	#define xx(a,b,c,d,e,f) d,
	#define yy(a,b,c,d,e,f) d,
	#include "token.h"
};
class Expr {
public:
	const int MAXPRIO = 13;
	Lexical* lex;
	Error* error;
	Expr(Lexical* _lex, Error* _error) { lex = _lex; error = _error; }
	/*--------------------------------[ Update Tree ]--------------------------------*/
	inline Tree*& updateTree(Tree*& p){
		Tree* tmp = new Tree;
		tmp->kid[0] = p; p = tmp;				//更新树
		p->op = lex->token; lex->getToken();
		return tmp->kid[1];
	}
	/*--------------------------------[ 表达式(主) ]--------------------------------*/
	Tree* expr() {
		Tree* p = exprAssign();
		if (lex->token == ',') updateTree(p) = expr();
		return p;
	}
	/*--------------------------------[ 赋值表达式 ]--------------------------------*/
	Tree* exprAssign() {
		Tree* p = exprBinary(4);
		if (lex->token == '=') updateTree(p) = exprAssign();
		return p;
	}
	/*--------------------------------[ 二元表达式 ]--------------------------------*/
	Tree* exprBinary(int pr) {
		Tree* p = exprUnary();
		for (int prt = prio[lex->token]; prt >= pr; prt--) 
			while (prio[lex->token] == prt) updateTree(p) = exprBinary(prt + 1);
		return p;
	}
	/*--------------------------------[ 一元表达式 ]--------------------------------*/
	Tree* exprUnary() {
		Tree* p = new Tree;
		switch (lex->token) {
		case '~': case '!': case '-': case ADDONE: case SUBONE:
			p->op = lex->token; lex->getToken(); 
			p->kid[0] = exprUnary();  break;
		case '*': {; } break;
		case '&': {; } break;
		case '(': {; } break;
		case SIZEOF: {; } break;
		default: delete p; p = exprPrimary();
		}
		return p;
	}
	/*--------------------------------[ 基础表达式 ]--------------------------------*/
	Tree* exprPrimary() {
		Tree* p = new Tree;
		switch (lex->token) {
		case ID: {									//变量
			p->op = lex->token;
			p->u.sym = SymTablePos->searchAllTable(lex->buffer.s);		//创建符号结构体;	符号表搜寻目标符号;	树节点值存储该符号
			error->error(p->u.sym == NULL, "Undeclared Identifier!");
			break;
		}
		case NUM:									//常数
		case REAL:
			p->op  = lex->token;
			p->u.v = lex->buffer;
			break;
		case STR: {; }
				break;
		case '(':									//括号
			lex->getToken();
			delete p;
			p = exprBinary(4);
			error->expect(')');
			break;
		}
		lex->getToken();
		return p;
	}

};
#endif