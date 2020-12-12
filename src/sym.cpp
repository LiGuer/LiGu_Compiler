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
#include "c.h"
/*::::::::::sym->符号表::::::::::*/
enum { CONSTANTS = 1, LABELS, GLOBAL, PARAM, LOCAL };//作用域
int cur = 0;
Table GlobalTable{ GLOBAL };
void Sym_insertGTable(Symbol* sym) {		//插入_全局符号表
	if (GlobalTable.sym[0] == NULL) {
		GlobalTable.sym[0] = sym;
		GlobalTable.sym[0];
		return;
	}
	Symbol* temp = GlobalTable.sym[0];
	while (temp->next != NULL) {
		temp = temp->next;
	}
	temp->next = sym;
}
Symbol* Sym_findID(const char* name) {		//查找_全局符号表
	Symbol* temp = GlobalTable.sym[0];
	while (temp != NULL) {
		if (*temp->name == *name) {
			return temp;
		}
		temp = temp->next;
	}return NULL;
}
int Sym_genLabel(int n) {				//生成跳转点标号
	static int label = 1;
	label += n;
	return label - n;
}