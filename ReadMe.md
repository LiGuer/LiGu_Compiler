# LiGu_Compiler
## List
Class:
* <Compiler>	Compiler编译器
* <SymbolTable>	符号表
* <Lexical>	    词法分析器
* <Parse>		语法分析器
* <Expr>		表达式__语法分析器
* <Decl>		声明__语法分析器
* <Stmt>		控制语句__语法分析器
* <Error>		报错
* <RI>			中间代码生成器 (RI: LLVM)
Other:
* <Tree>		抽象语法树
* <token.h>     符号
* <main.cpp>	主函数

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
{
	int a;
	double b;
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
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
%a = alloca int32, align 0
%b = alloca float64, align 0
%c = alloca int8, align 0
%d = alloca float32, align 0
%i = alloca int32, align 0
label 1: 
%1 = < i8 %c i32 %a
ifFalse %1 goto label 2
%2 = + f64 %b i32 %a
store %2 *i8 %c
goto label 1
label 2:
store 0 *i32 %i
label 3: 
%3 = < i32 %i 5
ifFalse %3 goto label 4
%4 = / i32 %a 27.710000
%5 = * f64 %b 0.120000
%6 = + %4 %5
%7 = - %6 2
store %7 *i32 %a
%8 = > i32 %a f32 %d
ifFalse %8 goto label 5
goto label 4
label 5: 
%9 = + i8 %c i32 %a
store %9 *i8 %c
%10 = > i8 %c f64 %b
ifFalse %10 goto label 6
goto label 3
label 6: 
%11 = + f64 %b i32 %i
store %11 *f64 %b
%12 = + i32 %i 1
store %12 *i32 %i
goto label 3
label 4:
```

## Reference
[1] AT&T, Christopher W. Fraser, and David R. Hanson. 
