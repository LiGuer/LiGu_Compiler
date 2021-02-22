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
	program -> stmt
******************************************************************************/
#ifndef PARSE_H
#define PARSE_H
#include "lex.h"
#include "stmt.h"
#include "decl.h"
#include "error.h"
class Parse {
public:
	Lexical* lexical;
	Error* error;
	Stmt* stmt;
	Parse(Lexical* _lexical, Error* _error) {
		lexical = _lexical; error = _error;
		stmt = new Stmt(lexical, error);
	}
	/*--------------------------------[ 语法分析器(主) ]--------------------------------*/
	Tree* parse() {
		lexical->getToken();
		return stmt->stmts(0);
	}
};
#endif
