# LiGu_Compiler
## List
Class:
* \<Compiler\>	Compiler编译器
* \<SymbolTable\>	符号表
* \<Lexical\>	    词法分析器
* \<Parse\>		语法分析器
* \<Expr\>		表达式__语法分析器
* \<Decl\>		声明__语法分析器
* \<Stmt\>		控制语句__语法分析器
* \<Error\>		报错
* \<RI\>			中间代码生成器 (RI: LLVM)
Other:
* \<Tree\>		抽象语法树
* \<token.h\>     符号
* \<main.cpp\>	主函数

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

## example
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
		a = a / 27.71 + b * 12E-2 - 2;
		if(a > d) break;
		c = c + a;
		if(c > b) continue;
		b = b + i;
	}
}

======================================================
#1 = alloca int 32 align 0
#2 = alloca int 64 align 0
#3 = alloca float 64 align 0
#4 = alloca int 8 align 0
#5 = alloca float 32 align 0
#6 = alloca int 32 align 0

label 7: 
#13 = < #4 #2
ifFalse #13 goto label 8
#14 = + #3 #2
store #14 *#4
goto label 7

label 8:
store 0 *#6

label 9: 
#15 = < #6 5
ifFalse #15 goto label 10
#16 = / #2 27.710000
#17 = * #3 0.120000
#18 = + #16 #17
#19 = - #18 2
store #19 *#2
#20 = > #2 #5
ifFalse #20 goto label 11
goto label 10

label 11: 
#21 = + #4 #2
store #21 *#4
#22 = > #4 #3
ifFalse #22 goto label 12
goto label 9

label 12: 
#23 = + #3 #6
store #23 *#3
#24 = + #6 1
store #24 *#6
goto label 9

label 10:

```

## Reference
[1] AT&T, Christopher W. Fraser, and David R. Hanson. 
