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
*                    Symble Table 符号表
-------------------------------------------------------------------------------
* 数据结构
Type 类型
Value 值
Symbol: [1] 符号名 [2] 类型
SymbolTable: [1] 符号Hash表 [2] 链表指针
******************************************************************************/
#ifndef SYM_H
#define SYM_H
#include<string.h>
class Type {
public:
	int type = 0;				//类型
	int size = 0;				//大小
	int sclass;					//类型扩展
	int align = 0;				//对齐
	bool constant = false;		//常数
	bool sign = true;			//正负
};
union Value {
	long long i;
	double d;
	void* p;
	char* s;
};
class Symbol { 
public:
	char* name = NULL;	Type* type = NULL; int label = 0;
};

class SymbolTable {
public:
	Symbol symtable[256];
	SymbolTable* prev = NULL;
	/*---------------- 基础函数 ----------------*/
	int getHash(const char* name) {
		int hash = 0;
		for (int i = 0; i < strlen(name); i++) hash = hash * 37 + name[i];	//Hash function
		return hash % 256;
	}
	// 插入
	bool insert(Symbol& sym) {
		// check exist
		if (search(sym.name) != NULL)return false;
		// save Symbol
		int hash = getHash(sym.name);
		while (symtable[hash].name != NULL) hash = (hash + 1) % 256; //如果该位置已有，则向下,若至表底则回开头
		symtable[hash] = sym;
		return true;
	}
	// 查找
	Symbol* search(const char* name) {
		int hash = getHash(name), hash0 = hash;
		while (symtable[hash].name != NULL && strcmp(symtable[hash].name, name) != 0) {
			hash = (hash + 1) % 256;
			if (hash = hash0)return NULL;
		}
		if (symtable[hash].name == NULL)return NULL;
		return &symtable[hash];
	}
	Symbol* searchAllTable(const char* name) {
		SymbolTable* table = this;
		Symbol* ans;
		while (table != NULL) {
			ans = table->search(name);
			if (ans != NULL)return ans;
			table = table->prev;
		}
		return NULL;
	}
};
SymbolTable GlobalTable;
SymbolTable* SymTablePos = &GlobalTable;

//生成跳转点标号
int Sym_genLabel(int n) {		
	static int label = 1;
	label += n;
	return label - n;
}
#endif