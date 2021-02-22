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
******************************************************************************/
#ifndef LEX_H
#define LEX_H
#include"sym.h"
enum {
#define xx(a,b,c,d,e) a = b,
#define yy(a,b,c,d,e)
#include "token.h"
};
class Lexical {
public:
	char token;
	Value buffer;
	char* codePos, *initialCodePos;
	/*---------------- 基础函数 ----------------*/
	Lexical(char* _codePos) {
		codePos = _codePos; initialCodePos = codePos;
	}
	// 数字、a-z大小写
	bool judgeCharacter(char c) {					
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')) return false;
		return true;
	}
	// 分析下一词
	char getToken() {
		token = lexicalAnalysis();
		return token;
	}
	// 词法分析
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
				if (codePos[0] == 'r'					//break
					&& codePos[1] == 'e'
					&& codePos[2] == 'a'
					&& codePos[3] == 'k'
					&& judgeCharacter(codePos[4])) {
					codePos = codePos + 4;
					return BREAK;
				}
				goto id;
			case 'c':									//[ C ]
				if (codePos[0] == 'a'					//case
					&& codePos[1] == 's'
					&& codePos[2] == 'e'
					&& judgeCharacter(codePos[3])) {
					codePos = codePos + 3;
					return CASE;
				}
				if (codePos[0] == 'h'					//char
					&& codePos[1] == 'a'
					&& codePos[2] == 'r'
					&& judgeCharacter(codePos[3])) {
					codePos = codePos + 3;
					return CHAR;
				}
				if (codePos[0] == 'l'					//class
					&& codePos[1] == 'a'
					&& codePos[2] == 's'
					&& codePos[3] == 's'
					&& judgeCharacter(codePos[4])) {
					codePos = codePos + 4;
					return CLASS;
				}
				if (codePos[0] == 'o'					//const
					&& codePos[1] == 'n'
					&& codePos[2] == 's'
					&& codePos[3] == 't'
					&& judgeCharacter(codePos[4])) {
					codePos = codePos + 4;
					return CONST;
				}
				if (codePos[0] == 'o'					//continue
					&& codePos[1] == 'n'
					&& codePos[2] == 't'
					&& codePos[3] == 'i'
					&& codePos[4] == 'n'
					&& codePos[5] == 'u'
					&& codePos[6] == 'e'
					&& judgeCharacter(codePos[7])) {
					codePos = codePos + 7;
					return CONTINUE;
				}
				goto id;
			case 'd':								//[ D ]
				if (codePos[0] == 'e'					//default
					&& codePos[1] == 'f'
					&& codePos[2] == 'a'
					&& codePos[3] == 'u'
					&& codePos[4] == 'l'
					&& codePos[5] == 't'
					&& judgeCharacter(codePos[6])) {
					codePos = codePos + 6;
					return DEFAULT;
				}
				goto id;
			case 'e':									//[ E ]
				if (codePos[0] == 'l'					//else
					&& codePos[1] == 's'
					&& codePos[2] == 'e'
					&& judgeCharacter(codePos[3])) {
					codePos = codePos + 3;
					return ELSE;
				}
				if (codePos[0] == 'n'					//enum
					&& codePos[1] == 'u'
					&& codePos[2] == 'm'
					&& judgeCharacter(codePos[3])) {
					codePos = codePos + 3;
					return ENUM;
				}
				if (codePos[0] == 'n'					//enum
					&& codePos[1] == 'u'
					&& codePos[2] == 'm'
					&& judgeCharacter(codePos[3])) {
					codePos = codePos + 3;
					return ENUM;
				}
				goto id;
			case 'f':									//[ F ]
				if (codePos[0] == 'l'					//float
					&& codePos[1] == 'o'
					&& codePos[2] == 'a'
					&& codePos[3] == 't'
					&& judgeCharacter(codePos[4])) {
					codePos = codePos + 4;
					return FLOAT;
				}
				if (codePos[0] == 'l'					//float
					&& codePos[1] == 'o'
					&& codePos[2] == 'a'
					&& codePos[3] == 't'
					&& codePos[4] == '6'
					&& codePos[5] == '4'
					&& judgeCharacter(codePos[6])) {
					codePos = codePos + 6;
					return FLOAT64;
				}
				if (codePos[0] == 'o'					//for
					&& codePos[1] == 'r'
					&& judgeCharacter(codePos[2])) {
					codePos = codePos + 2;
					return FOR;
				}
				goto id;
			case 'i':									//[ I ]
				if (codePos[0] == 'f'					//if
					&& judgeCharacter(codePos[1])) {
					codePos = codePos + 1;
					return IF;
				}
				if (codePos[0] == 'n'					//int
					&& codePos[1] == 't'
					&& judgeCharacter(codePos[2])) {
					codePos = codePos + 2;
					return INT;
				}
				if (codePos[0] == 'n'					//int
					&& codePos[1] == 't'
					&& codePos[2] == '6'
					&& codePos[3] == '4'
					&& judgeCharacter(codePos[4])) {
					codePos = codePos + 4;
					return INT64;
				}
				goto id;
			case 'r':									//[ R ]
				if (codePos[0] == 'e'					//return
					&& codePos[1] == 't'
					&& codePos[2] == 'u'
					&& codePos[3] == 'r'
					&& codePos[4] == 'n'
					&& judgeCharacter(codePos[5])) {
					codePos = codePos + 5;
					return RETURN;
				}
				goto id;
			case 's':									//[ S ]
				if (codePos[0] == 'h'					//short
					&& codePos[1] == 'o'
					&& codePos[2] == 'r'
					&& codePos[3] == 't'
					&& judgeCharacter(codePos[4])) {
					codePos = codePos + 4;
					return SHORT;
				}
				if (codePos[0] == 't'					//static
					&& codePos[1] == 'a'
					&& codePos[2] == 't'
					&& codePos[3] == 'i'
					&& codePos[4] == 'c'
					&& judgeCharacter(codePos[5])) {
					codePos = codePos + 5;
					return STATIC;
				}
				if (codePos[0] == 'w'					//switch
					&& codePos[1] == 'i'
					&& codePos[2] == 't'
					&& codePos[3] == 'c'
					&& codePos[4] == 'h'
					&& judgeCharacter(codePos[5])) {
					codePos = codePos + 5;
					return SWITCH;
				}
				goto id;
			case 'u':									//[ U ]
				if (codePos[0] == 'n'					//unsigned
					&& codePos[1] == 's'
					&& codePos[2] == 'i'
					&& codePos[3] == 'g'
					&& codePos[4] == 'n'
					&& codePos[5] == 'e'
					&& codePos[6] == 'd'
					&& judgeCharacter(codePos[6])) {
					codePos = codePos + 6;
					return UNSIGNED;
				}
				goto id;
			case 'w':									//[ W ]
				if (codePos[0] == 'h'					//while
					&& codePos[1] == 'i'
					&& codePos[2] == 'l'
					&& codePos[3] == 'e'
					&& judgeCharacter(codePos[4])) {
					codePos = codePos + 4;
					return WHILE;
				}
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


