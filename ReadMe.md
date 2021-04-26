# LiGu_Compiler
## Codes List
Class:
* \<Compiler\>		Compiler编译器
* \<SymbolTable\>	符号表
* \<Lexical\>	    词法分析
* \<Parse\>			语法分析——main
* \<Expr\>			语法分析——表达式
* \<Decl\>			语法分析——声明
* \<Stmt\>			语法分析——控制语句
* \<Error\>			报错
* \<RI\>			中间代码生成
Other:
* \<Tree\>			抽象语法树
* \<token.h\>		符号
* \<main.cpp\>		主函数

## Achievement
* 变量声明
* {} 代码块
* int double... 类型
* +-*/()%表达式
* if 语句
* while 语句
* for 语句
* break;continue;
* // /**/注释
* 转中间代码

## Copyright
The authors of this software are Christopher W. Fraser and David R. Hanson.  
Copyright (c) 1991,1992,1993,1994,1995,1996,1997,1998,1999,2000,2001,2002 by AT&T, Christopher W. Fraser, and David R. Hanson.   
Copyright (c) 2020 LiGuer.   
All Rights Reserved.  

## Example
```
// Test LiGu's Compiler
/* 2020-12 */
int a;
{
	int64 a;
	float64 b;
	char c;
	float d;
	int i;
	while(c < a) c = b + a;
	for(i = 0; i < 5; i = i + 1){
		a = a / 27.71 + b * 12E-2 - 2 << 7;
		if(a > d && a <= c) break;
		c = c + a;
		if(c > b) continue;
		b = b + i;
	}
}
======================================================
#1	INTEG ALLOC 32 ALIGN 0
#2	INTEG ALLOC 64 ALIGN 0
#3	FLOAT ALLOC 64 ALIGN 0
#4	INTEG ALLOC 8 ALIGN 0
#5	FLOAT ALLOC 32 ALIGN 0
#6	INTEG ALLOC 32 ALIGN 0

label7: 
MOV		#13 #4
LCM		#13 #2
JMPFalse	#13 label8
MOV		#14 #3
ADD		#14 #2
STORE	#14 *#4
JMP	label7

label8:
STORE	0 *#6

label9: 
MOV		#15 #6
LCM		#15 5
JMPFalse	#15 label10
MOV		#16 #2
DIV		#16 27.710000
MOV		#17 #3
MUL		#17 0.120000
MOV		#18 #16
ADD		#18 #17
MOV		#19 #18
SUB		#19 2
MOV		#20 #19
LSH		#20 7
STORE	#20 *#2
MOV		#21 #2
GCM		#21 #5
MOV		#22 #2
LEC		#22 #4
MOV		#23 #21
LOGAND		#23 #22
JMPFalse	#23 label11
JMP	label10

label11: 
MOV		#24 #4
ADD		#24 #2
STORE	#24 *#4
MOV		#25 #4
GCM		#25 #3
JMPFalse	#25 label12
JMP	label9

label12: 
MOV		#26 #3
ADD		#26 #6
STORE	#26 *#3
MOV		#27 #6
ADD		#27 1
STORE	#27 *#6
JMP	label9

label10:

```

## Reference
[1] AT&T, Christopher W. Fraser, and David R. Hanson. 
