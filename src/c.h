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
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include "struct.h"
enum {
#define xx(a,b,c,d,e) a = b,
#define yy(a,b,c,d,e)
#include "token.h"
};
// 面向对象思想，一个".c "即一个类，方法细节对内封装。下面是类的对外接口。
//---------------- main.c ----------------
extern FILE* fout;
extern char* codePos;
int main(int argc, char* argv[]);
//---------------- lex.c ----------------
extern Value lexBuffer;
int lex(Value& lexBuffer);
//---------------- parse.c ----------------
extern int token;
Tree* parse();
Tree* block();
//---------------- sym.c ----------------
void Sym_insertGTable(Symbol* sym);
Symbol* Sym_findID(const char* name);
int Sym_genLabel(int n);
//---------------- decl.c ----------------
Tree* decl();
//---------------- expr.c ----------------
Tree* expr();
//---------------- stmt.c ----------------
Tree* stmt();
//---------------- error.c ----------------
void error(const char err[], ...);
void expect(int tok);
//---------------- IR.c ----------------
void IRgen(Tree* p);