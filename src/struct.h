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
union Value {
	long long i;
	unsigned long u;
	long double d;
	void* p;
	char* s;
};
struct Type {
	int type;	//类型
	int size;	//大小
	int sclass;	//类型扩展
	int align;	//对齐
	int cons;
	int sign;
};
/*:::::::::::::::sym.c:::::::::::::::*/
struct Symbol {				//符号结构体
	char* name;				//符号名
	int scope;				//作用域
	Type* type;				//类型
	Symbol* next=NULL;			//链表指针
};
struct Table {				//符号表(哈希表)
	int level;				//作用域
	Symbol* sym[256];
	Table* prev;			//链表指针(向外层)
};
struct Tree {							//Tree(op,val,*kid[2])
	int op = 0;
	union {
		Value v;
		Symbol* sym;
	}u;
	Tree* kid[2] = { NULL,NULL };
};