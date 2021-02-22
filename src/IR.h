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
*                    IR 中间代码生成
-------------------------------------------------------------------------------
[语法] LLVM IR语法
-------------------------------------------------------------------------------
* Identifiers 标识符
	@name 全局	%name 局部
-------------------------------------------------------------------------------
* Terminator Instructions 终端指令
	ret <type> <value> / void							;返回
	br i1 <cond>, label <iftrue>, label <iffalse>		;二元条件分支
	br label <dest>										;无条件跳转
	switch <intty> <value>, label <defaultdest> [ <intty> <val>, label <dest> ... ]
														;多条件分支
	indirectbr <somety>* <address>, [ label <dest1>, label <dest2>, ... ]
														;跳转到指定地址基本块中
	invoke												;普通/带异常调用
	resume												;抛出异常?
	catchswitch											;捕获异常
	catchret
	cleanupret
	unreachable											;不可到达(无语义)
-------------------------------------------------------------------------------
* Unary Operations 一元运算
	fneg: <result> = fneg [fast-math flags]* <ty> <op1>	;相反数
* Binary Operations 二元运算
	add:  <result> = add (nuw nsw) <ty> <op1>, <op2>	;加 ;yields ty:result  (nuw:No Unsigned Wrap; nsw:No Signed Wrap)
	fadd: <result> = fadd [fast-math flags]* <ty> <op1>, <op2>	;浮点加  
	sub:  <result> = sub (nuw nsw) <ty> <op1>, <op2>	;减
	fsub: <result> = fsub [fast-math flags]* <ty> <op1>, <op2>	;浮点减  
	mul													;乘
	fmul												;浮点乘
	udiv: <result> = udiv (exact) <ty> <op1>, <op2>		;无符号整数除
	sdiv: <result> = sdiv i32 4, %var					;带符号整数除 ; yields i32:result = 4 / %var 
	fdiv												;浮点除
	urem												;无符号整数求余
	srem												;带符号整数求余
	frem												;浮点数求余
-------------------------------------------------------------------------------
* Bitwise Binary Operations 二进制位运算
	shl:  <result> = shl (nuw nsw) <ty> <op1>, <op2>	;左移
	lshr: <result> = lshr (exact) <ty> <op1>, <op2>		;逻辑右移
	ashr: <result> = ashr (exact) <ty> <op1>, <op2>		;算数右移
	and:  <result> = and <ty> <op1>, <op2>				;与
	or:   <result> = or <ty> <op1>, <op2>				;或
	xor:  <result> = xor <ty> <op1>, <op2>				;异或
-------------------------------------------------------------------------------
* Vector Operations 向量运算
	extractelement										;取出元素
		<result> = extractelement <(vscale x) n x <ty>> <val>, <ty2> <idx>  ; yields <ty>
	insertelement 										;插入元素
		<result> = insertelement <(vscale x) n x <ty>> <val>, <ty> <elt>, <ty2> <idx>    ; yields <n x <ty>>
	shufflevector										;从两个向量中抽取指定的一些元素
		<result> = shufflevector <(vscale x) n x <ty>> <v1>, <(vscale x) n x <ty>> <v2>, <m x i32> <mask>    ; yields <m x <ty>>
-------------------------------------------------------------------------------
* Aggregate Operations 聚合运算
	extractvalue										;取出值
		<result> = extractvalue <aggregate type> <val>, <idx>{, <idx>}*
	insertvalue											;插入值
		<result> = insertvalue <aggregate type> <val>, <ty> <elt>, <idx>{, <idx>}*    ; yields <aggregate type>
-------------------------------------------------------------------------------
* Memory Access and Addressing Operations 内存访问和寻址
	alloca												;分配内存
		<result> = alloca [inalloca] <type> [, <ty> <NumElements>] [, align <alignment>] [, addrspace(<num>)] 
	load												;从内存加载
		<<result> = load [volatile] <ty>, <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>][, !invariant.load !<index>][, !invariant.group !<index>][, !nonnull !<index>][, !dereferenceable !<deref_bytes_node>][, !dereferenceable_or_null !<deref_bytes_node>][, !align !<align_node>]
		<result> = load atomic [volatile] <ty>, <ty>* <pointer> [syncscope("<target-scope>")] <ordering>, align <alignment> [, !invariant.group !<index>]
		!<index> = !{ i32 1 }
		!<deref_bytes_node> = !{i64 <dereferenceable_bytes>}
		!<align_node> = !{ i64 <value_alignment> }
	store												;储存到内存
		store (atomic) [volatile] <ty> <value>, <ty>* <pointer> [(syncscope("<target-scope>")] <ordering>), align <alignment> [, !invariant.group !<index>]
	fence
	cmpxchg
	atomicrmw											;自动修改内存
	getelementptr										;获取 aggregate(集合) 数据结构的子成员地址
		<result> = getelementptr (inbounds) <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
		<result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>
-------------------------------------------------------------------------------
Coversion Operations 转换操作
	trunc .. to: <result> = trunc <ty> <value> to <ty2>	;截断转换
	zext .. to											;零扩展转换
	sext .. to             								;符号位扩展转换
	fptrunc .. to          								;浮点截断转换
	fpext .. to            								;浮点扩展
	fptoui .. to           								;浮点转无符号整数
	fptosi .. to           								;浮点转带符号整数
	uitofp .. to           								;无符号整数转浮点
	sitofp .. to           								;带符号整数转浮点
	ptrtoint .. to         								;指针转整数
	inttoptr .. to         								;整数转指针
	bitcast .. to          								;位模式转换(重新解释，不改变任何二进制位)
	addrepacecast .. to    								;地址空间转换
-------------------------------------------------------------------------------
Other Operations
	icmp: <result> = icmp <cond> <ty> <op1>, <op2>		;整数比较
	fcmp          										;浮点数比较
		<result> = fcmp [fast-math flags]* <cond> <ty> <op1>, <op2>     ; yields i1 or <N x i1>:result
	phi           										;φ 节点
		<result> = phi [fast-math-flags] <ty> [ <val0>, <label0>], ...
	select        										;条件值选择
		<result> = select [fast-math flags] selty <cond>, <ty> <val1>, <ty> <val2>   
	call          										;简单函数调用
		<result> = [tail | musttail | notail ] call [fast-math flags] [cconv] [ret attrs] [addrspace(<num>)]<ty>|<fnty> <fnptrval>(<function args>) [fn attrs] [ operand bundles ]
	va_arg        										;可变参数
	landingpad											;接收异常信息
	catchpad											;异常处理机制
	cleanuppad											;异常处理机制
******************************************************************************/
const char* OpName[] = {
#define xx(a,b,c,d,e) c,
#define yy(a,b,c,d,e) c,
#include "token.h"
};
class IR {
public:
	int idCur = 1;
	FILE* fout;
	/*--------------------------------[ IR生成 ]--------------------------------*/
	void IRgen(Tree* p, const char* output) { 
		fout = fopen(output, "w+");
		walkTree(p);
		fclose(fout);
	}
	/*--------------------------------[ 遍历树 ]--------------------------------*/
	char* walkTree(Tree* p) {
		
		char* childLeft = NULL, * childRight = NULL;
		if (p->op == WHILE) { fprintf(fout, "\nlabel %d: \n", p->u.v.i); }
		// Left
		if (p->kid[0] != NULL) { childLeft = walkTree(p->kid[0]); }
		if (p->op == IF) { fprintf(fout, "  ifFalse %s goto label %d\n", childLeft, p->u.v.i); }
		else  if (p->op == WHILE) { fprintf(fout, "  ifFalse %s goto label %d\n", childLeft, p->u.v.i + 1); }
		// Right
		if (p->kid[1] != NULL) { childRight = walkTree(p->kid[1]); }
		if (p->op == 0)return childLeft;
		else if (p->op < 18) {
			printf(" %%%s\n", p->u.sym->name);
			fprintf(fout, "  %%%s = alloca %s%d, align %d\n",p->u.sym->name, OpName[p->u.sym->type->type],p->u.sym->type->size, p->u.sym->type->align); return childLeft; }
		else if (p->op == '=') { fprintf(fout, "  store %s *%s\n", childRight, childLeft); return childLeft; }
		else if (p->op == IF) { fprintf(fout, "\nlabel %d: \n", p->u.v.i); }
		else if (p->op == WHILE) { fprintf(fout, "  goto label %d\n", p->u.v.i);	fprintf(fout, "\nlabel %d:\n", p->u.v.i + 1); }
		else if (p->op == BREAK) { fprintf(fout, "  goto label %d\n", p->u.v.i); }
		else if (p->op == REAL) return double2String(p->u.v.d);
		else if (p->op == NUM) return int2string(p->u.v.i);
		else if (p->op == ID) {
			char* t = (char*)malloc(10 * sizeof(char));
			int cur = 0;
			if (p->u.sym->type->type == INT)t[cur++] = 'i';
			else if (p->u.sym->type->type == FLOAT)t[cur++] = 'f';
			if (p->u.sym->type->size != 0) {
				char* size = int2string(p->u.sym->type->size);
				int sizecur = 0;
				while (size[sizecur] != '\0')t[cur++] = size[sizecur++];
				t[cur++] = ' ';
			}
			t[cur++] = '%';
			int namecur = 0;
			while (p->u.sym->name[namecur] != '\0')t[cur++] = p->u.sym->name[namecur++];
			t[cur++] = '\0';
			return t;
		}
		else {
			char* t = (char*)malloc(10 * sizeof(char));
			int cur = 0;
			t[cur++] = '%';
			char* ts = int2string(idCur++);
			int tscur = 0;
			while (ts[tscur] != '\0')t[cur++] = ts[tscur++];
			t[cur++] = '\0';
			fprintf(fout, "  %s = %s %s %s\n", t, OpName[p->op], childLeft, childRight);
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
