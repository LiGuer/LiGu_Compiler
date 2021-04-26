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
*                    Lexical Analysis 词法分析器
*	[词]:
		[1] 空格  [2] 0 - 9  [3] 运算符  [4] A - Z
*	[算法]: 有限状态机 (基于switch-case实现)
******************************************************************************/
#ifndef LEX_H
#define LEX_H
#include"sym.h"
enum {
#define xx(a,b,c,d,e,f) a = b,
#define yy(a,b,c,d,e,f)
#include "token.h"
};
class Lexical {
public:
	char token;
	Value buffer;
	char* codePos, *initialCodePos;
	/*---------------- 构造/析构函数 ----------------*/
	Lexical(char* _codePos) {
		codePos = _codePos; initialCodePos = codePos;
	}
	/*---------------- 判断数字、a-z大小写 ----------------*/
	inline static bool judgeCharacter(char c) {
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) return false;
		return true;
	}
	/*---------------- 判断指令 ----------------*/
	inline static bool CodeCmp(const char* code, char*& data) {
		int index = 0;
		while (code[index] && code[index] == data[index - 1]) { index++; }
		if (code[index] == '\0' && judgeCharacter(data[index - 1])) {
			data += index - 1; return true;
		}
		return false;
	}
	/*---------------- 分析下一词 ----------------*/
	char getToken() { return token = lexicalAnalysis(); }
	/*---------------- 词法分析 ----------------*/
	char lexicalAnalysis() {
		while (true) {
			switch (*codePos++) {
			case EOI:return EOI;
			//----------------[ 空格 ]----------------
			case '\n':case '\r':case '\t':case ' ': continue;
			//----------------[ 0 - 9 ]----------------
			case '0':case '1':case '2':case '3':case '4':
			case '5':case '6':case '7':case '8':case '9': {
				char* tmp = codePos - 1;		//整数
				int num = 0;
				while (*tmp >= '0' && *tmp <= '9') {
					num = num * 10 + *tmp - '0';
					tmp++;
				}
				if (*tmp == '.' || *tmp == 'e' || *tmp == 'E')goto REAL;
				codePos = tmp;
				buffer.i = num;
				return NUM;
			}REAL: {								//实数
				char* temp = codePos - 1;
				double real = 0;
				while (*temp >= '0' && *temp <= '9') {
					real = real * 10 + *temp - '0';
					temp++;
				}
				if (*temp == '.') {					//小数点
					temp++; double cur = 0.1;
					while (*temp >= '0' && *temp <= '9') {
						real += (*temp - '0') * cur;
						cur *= 0.1;
						temp++;
					}
				}
				else if (*temp == 'e' || *temp == 'E') {	//科学计数法
					temp++; double cur = 1, flag = 1;
					if (*temp == '-') { flag = -1; temp++; }
					else if (*temp == '+') temp++;
					while (*temp >= '0' && *temp <= '9') {
						int i = *temp - '0';
						while (i--)cur *= 10;
						temp++;
					}
					if (flag == -1)cur = 1 / cur;
					real *= cur;
				}
				codePos = temp;
				buffer.d = real;
				return REAL;
			}
			//----------------[ 运算符 ]----------------
			case '<':
				if (*codePos == '=')return codePos++, LEQ;			// <=
				if (*codePos == '<')return codePos++, LSHIFT;		// <<
				return '<';											// <
			case '>':
				if (*codePos == '=')return codePos++, GEQ;			// >=
				if (*codePos == '>')return codePos++, RSHIFT;		// >>
				return '>';											// <
			case '/':
				if (*codePos == '/') {								// "//"
					while (*codePos++ != '\n') {}
					continue;
				}
				if (*codePos == '*') {								// "/*"
					while (codePos[1] != '*' && codePos[2] != '/') codePos++;
					codePos += 3;
					continue;
				}
				else return '/';									// "/"
			case '+':return *codePos == '+' ? codePos++, ADDONE : '+';	// ++ , +
			case '-':return *codePos == '-' ? codePos++, SUBONE : '-';	// -- , -
			case '=':return *codePos == '=' ? codePos++, EQL : '=';	// == , =
			case '!':return *codePos == '=' ? codePos++, NQL : '!';	// != , !
			case '&':return *codePos == '&' ? codePos++, ANDAND : '&';	// && , &
			case '|':return *codePos == '|' ? codePos++, OROR : '|';	// || , |
			case '*':case '^':case '%':								// * , ^ , %
			case ',':case ':':case ';':								// , , : , ;
			case '(':case ')':case '[': case ']': case '{': case '}'://括号 () , [] , {}
				return codePos[-1];
			//----------------[ A - Z ]----------------
			case 'b':									//[ B ]
				if (CodeCmp("break",	codePos)) return BREAK;		//break
				goto id;
			case 'c':									//[ C ]
				if (CodeCmp("case",		codePos)) return CASE;		//case
				if (CodeCmp("char",		codePos)) return CHAR;		//char
				if (CodeCmp("class",	codePos)) return CLASS;		//class
				if (CodeCmp("class",	codePos)) return CLASS;		//class
				if (CodeCmp("const",	codePos)) return CONST;		//const
				if (CodeCmp("continue",	codePos)) return CONTINUE;	//continue
				goto id;
			case 'd':								//[ D ]
				if (CodeCmp("default",	codePos)) return DEFAULT;	//default
				goto id;
			case 'e':									//[ E ]
				if (CodeCmp("else",		codePos)) return ELSE;		//else
				if (CodeCmp("enum",		codePos)) return ENUM;		//enum
				goto id;
			case 'f':									//[ F ]
				if (CodeCmp("float",	codePos)) return FLOAT;		//float
				if (CodeCmp("float64",	codePos)) return FLOAT64;	//float64
				if (CodeCmp("for",		codePos)) return FOR;		//for
				goto id;
			case 'i':									//[ I ]
				if (CodeCmp("if",		codePos)) return IF;		//if
				if (CodeCmp("int",		codePos)) return INT;		//int
				if (CodeCmp("int64",	codePos)) return INT64;		//int64
				goto id;
			case 'r':									//[ R ]
				if (CodeCmp("return",	codePos)) return RETURN;	//return
				goto id;
			case 's':									//[ S ]
				if (CodeCmp("short",	codePos)) return SHORT;		//short
				if (CodeCmp("sizeof",	codePos)) return SIZEOF;	//sizeof
				if (CodeCmp("static",	codePos)) return STATIC;	//static
				if (CodeCmp("switch",	codePos)) return SWITCH;	//switch
				goto id;
			case 'u':									//[ U ]
				if (CodeCmp("unsigned",	codePos)) return UNSIGNED;	//unsigned
				goto id;
			case 'w':									//[ W ]
				if (CodeCmp("while",	codePos)) return WHILE;		//while
				goto id;
			case 'a':
			case 'g': case 'h':			  case 'j': case 'k': case 'l':
			case 'm': case 'n': case 'o': case 'p': case 'q':
			case 't':			case 'v':           case 'x':
			case 'y': case 'z':
			case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
			case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
			case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
			case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
			case 'Y': case 'Z':
			id: {										//变量
				static const int maxNameLen = 100;
				buffer.s = (char*)malloc(maxNameLen * sizeof(char));
				int cur = 0;
				while (!judgeCharacter(*(codePos + cur - 1))) {
					buffer.s[cur++] = *(codePos + cur - 1);
				}buffer.s[cur] = '\0';
				codePos = codePos + cur - 1;
				return ID;
			}
			default: continue;
			}
		}
	}
};
#endif