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
*                    Error 报错
******************************************************************************/
#ifndef ERROR_H
#define ERROR_H
#include "lex.h"
class Error {
public:
	int errorNum = 0;
	int maxErrorNum = 10;
	Lexical* lexical;
	Error(Lexical* _lexical) { lexical = _lexical; }
	// error
	void error(const char* err, ...) {
		printf("error %d: %s in %d.\n", errorNum, err, lexical->codePos - lexical->initialCodePos);
		if (errorNum++ >= maxErrorNum) {
			printf("Too many errors\n");
			exit(1);
		}
	}
	// expect
	void expect(int tok) {
		if (lexical->token == tok) lexical->getToken();
		else {
			error("syntax error!");
			printf("	expecting '%c'\n", tok);
		}
	}
};
#endif
