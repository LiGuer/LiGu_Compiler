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
*						IR 中间代码生成
******************************************************************************/
const char* OpName[] = {
#define xx(a,b,c,d,e) c,
#define yy(a,b,c,d,e) c,
#include "token.h"
};
#include"sym.h"
class IR {
public:
	FILE* fout;
	/*--------------------------------[ IR���� ]--------------------------------*/
	void IRgen(Tree* p, const char* output) { 
		fout = fopen(output, "w+");
		walkTree(p);
		fclose(fout);
	}
	/*--------------------------------[ ������ ]--------------------------------*/
	char* walkTree(Tree* p) {
		if (p == NULL)return NULL;
		
		char* childLeft = NULL, * childRight = NULL;
		if (p->op == WHILE) { fprintf(fout, "\nlabel%d: \n", p->u.v.i); }

		// Left
		childLeft = walkTree(p->kid[0]);
		if (p->op == IF) { fprintf(fout, "JMPFalse %s label%d\n", childLeft, p->u.v.i); }
		else  if (p->op == WHILE) { fprintf(fout, "JMPFalse %s label%d\n", childLeft, p->u.v.i + 1); }

		// Right
		childRight = walkTree(p->kid[1]);
		if (p->op == 0)return childLeft;
		else if (p->op < 18) {fprintf(fout, "ALLOC #%d %s %d ALIGN %d\n", p->u.sym->label, OpName[p->u.sym->type->type], p->u.sym->type->size, p->u.sym->type->align); return childLeft;}
		else if (p->op == '=') { fprintf(fout, "STORE %s *%s\n", childRight, childLeft); return childLeft; }
		else if (p->op == IF) { fprintf(fout, "\nlabel%d: \n", p->u.v.i); }
		else if (p->op == WHILE) { fprintf(fout, "JMP label%d\n", p->u.v.i);	fprintf(fout, "\nlabel%d:\n", p->u.v.i + 1); }
		else if (p->op == BREAK) { fprintf(fout, "JMP label%d\n", p->u.v.i); }
		else if (p->op == REAL) return double2String(p->u.v.d);
		else if (p->op == NUM) return int2string(p->u.v.i);
		else if (p->op == ID) {
			char* t = (char*)malloc(10 * sizeof(char));
			int cur = 0;
			t[cur++] = '#';
			char* ts = int2string(p->u.sym->label);
			int tscur = 0;
			while (ts[tscur] != '\0')t[cur++] = ts[tscur++];
			t[cur++] = '\0';
			return t;
		}
		else {
			char* t = (char*)malloc(10 * sizeof(char));
			int cur = 0;
			t[cur++] = '#';
			char* ts = int2string(Sym_genLabel(1));
			int tscur = 0;
			while (ts[tscur] != '\0')t[cur++] = ts[tscur++];
			t[cur++] = '\0';
			fprintf(fout, "%s = %s %s %s\n", t, OpName[p->op], childLeft, childRight);
			return t;
		}
	}
	char* int2string(int n)
	{
		char* t = (char*)malloc(100 * sizeof(char));
		if (n == 0)return t = (char*)"0\0";
		int cur = 0;
		while (n) {
			t[cur++] = n % 10 + '0';
			n /= 10;
		}
		for (int i = 0; i < cur / 2; i++) {
			char temp = t[i];
			t[i] = t[cur - i - 1];
			t[cur - i - 1] = temp;
		}
		t[cur] = '\0';
		return t;
	}
	char* double2String(double a) {
		std::string str = std::to_string(a);
		char* t = (char*)malloc(sizeof(char) * (str.length() + 1));
		for (int i = 0; i < str.length(); i++)t[i] = str[i];
		t[str.length()] = '\0';
		return t;
	}
};
