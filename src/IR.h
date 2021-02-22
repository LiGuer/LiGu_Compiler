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
*                    IR �м��������
-------------------------------------------------------------------------------
[�﷨] LLVM IR�﷨
-------------------------------------------------------------------------------
* Identifiers ��ʶ��
	@name ȫ��	%name �ֲ�
-------------------------------------------------------------------------------
* Terminator Instructions �ն�ָ��
	ret <type> <value> / void							;����
	br i1 <cond>, label <iftrue>, label <iffalse>		;��Ԫ������֧
	br label <dest>										;��������ת
	switch <intty> <value>, label <defaultdest> [ <intty> <val>, label <dest> ... ]
														;��������֧
	indirectbr <somety>* <address>, [ label <dest1>, label <dest2>, ... ]
														;��ת��ָ����ַ��������
	invoke												;��ͨ/���쳣����
	resume												;�׳��쳣?
	catchswitch											;�����쳣
	catchret
	cleanupret
	unreachable											;���ɵ���(������)
-------------------------------------------------------------------------------
* Unary Operations һԪ����
	fneg: <result> = fneg [fast-math flags]* <ty> <op1>	;�෴��
* Binary Operations ��Ԫ����
	add:  <result> = add (nuw nsw) <ty> <op1>, <op2>	;�� ;yields ty:result  (nuw:No Unsigned Wrap; nsw:No Signed Wrap)
	fadd: <result> = fadd [fast-math flags]* <ty> <op1>, <op2>	;�����  
	sub:  <result> = sub (nuw nsw) <ty> <op1>, <op2>	;��
	fsub: <result> = fsub [fast-math flags]* <ty> <op1>, <op2>	;�����  
	mul													;��
	fmul												;�����
	udiv: <result> = udiv (exact) <ty> <op1>, <op2>		;�޷���������
	sdiv: <result> = sdiv i32 4, %var					;������������ ; yields i32:result = 4 / %var 
	fdiv												;�����
	urem												;�޷�����������
	srem												;��������������
	frem												;����������
-------------------------------------------------------------------------------
* Bitwise Binary Operations ������λ����
	shl:  <result> = shl (nuw nsw) <ty> <op1>, <op2>	;����
	lshr: <result> = lshr (exact) <ty> <op1>, <op2>		;�߼�����
	ashr: <result> = ashr (exact) <ty> <op1>, <op2>		;��������
	and:  <result> = and <ty> <op1>, <op2>				;��
	or:   <result> = or <ty> <op1>, <op2>				;��
	xor:  <result> = xor <ty> <op1>, <op2>				;���
-------------------------------------------------------------------------------
* Vector Operations ��������
	extractelement										;ȡ��Ԫ��
		<result> = extractelement <(vscale x) n x <ty>> <val>, <ty2> <idx>  ; yields <ty>
	insertelement 										;����Ԫ��
		<result> = insertelement <(vscale x) n x <ty>> <val>, <ty> <elt>, <ty2> <idx>    ; yields <n x <ty>>
	shufflevector										;�����������г�ȡָ����һЩԪ��
		<result> = shufflevector <(vscale x) n x <ty>> <v1>, <(vscale x) n x <ty>> <v2>, <m x i32> <mask>    ; yields <m x <ty>>
-------------------------------------------------------------------------------
* Aggregate Operations �ۺ�����
	extractvalue										;ȡ��ֵ
		<result> = extractvalue <aggregate type> <val>, <idx>{, <idx>}*
	insertvalue											;����ֵ
		<result> = insertvalue <aggregate type> <val>, <ty> <elt>, <idx>{, <idx>}*    ; yields <aggregate type>
-------------------------------------------------------------------------------
* Memory Access and Addressing Operations �ڴ���ʺ�Ѱַ
	alloca												;�����ڴ�
		<result> = alloca [inalloca] <type> [, <ty> <NumElements>] [, align <alignment>] [, addrspace(<num>)] 
	load												;���ڴ����
		<<result> = load [volatile] <ty>, <ty>* <pointer>[, align <alignment>][, !nontemporal !<index>][, !invariant.load !<index>][, !invariant.group !<index>][, !nonnull !<index>][, !dereferenceable !<deref_bytes_node>][, !dereferenceable_or_null !<deref_bytes_node>][, !align !<align_node>]
		<result> = load atomic [volatile] <ty>, <ty>* <pointer> [syncscope("<target-scope>")] <ordering>, align <alignment> [, !invariant.group !<index>]
		!<index> = !{ i32 1 }
		!<deref_bytes_node> = !{i64 <dereferenceable_bytes>}
		!<align_node> = !{ i64 <value_alignment> }
	store												;���浽�ڴ�
		store (atomic) [volatile] <ty> <value>, <ty>* <pointer> [(syncscope("<target-scope>")] <ordering>), align <alignment> [, !invariant.group !<index>]
	fence
	cmpxchg
	atomicrmw											;�Զ��޸��ڴ�
	getelementptr										;��ȡ aggregate(����) ���ݽṹ���ӳ�Ա��ַ
		<result> = getelementptr (inbounds) <ty>, <ty>* <ptrval>{, [inrange] <ty> <idx>}*
		<result> = getelementptr <ty>, <ptr vector> <ptrval>, [inrange] <vector index type> <idx>
-------------------------------------------------------------------------------
Coversion Operations ת������
	trunc .. to: <result> = trunc <ty> <value> to <ty2>	;�ض�ת��
	zext .. to											;����չת��
	sext .. to             								;����λ��չת��
	fptrunc .. to          								;����ض�ת��
	fpext .. to            								;������չ
	fptoui .. to           								;����ת�޷�������
	fptosi .. to           								;����ת����������
	uitofp .. to           								;�޷�������ת����
	sitofp .. to           								;����������ת����
	ptrtoint .. to         								;ָ��ת����
	inttoptr .. to         								;����תָ��
	bitcast .. to          								;λģʽת��(���½��ͣ����ı��κζ�����λ)
	addrepacecast .. to    								;��ַ�ռ�ת��
-------------------------------------------------------------------------------
Other Operations
	icmp: <result> = icmp <cond> <ty> <op1>, <op2>		;�����Ƚ�
	fcmp          										;�������Ƚ�
		<result> = fcmp [fast-math flags]* <cond> <ty> <op1>, <op2>     ; yields i1 or <N x i1>:result
	phi           										;�� �ڵ�
		<result> = phi [fast-math-flags] <ty> [ <val0>, <label0>], ...
	select        										;����ֵѡ��
		<result> = select [fast-math flags] selty <cond>, <ty> <val1>, <ty> <val2>   
	call          										;�򵥺�������
		<result> = [tail | musttail | notail ] call [fast-math flags] [cconv] [ret attrs] [addrspace(<num>)]<ty>|<fnty> <fnptrval>(<function args>) [fn attrs] [ operand bundles ]
	va_arg        										;�ɱ����
	landingpad											;�����쳣��Ϣ
	catchpad											;�쳣�������
	cleanuppad											;�쳣�������
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
	/*--------------------------------[ IR���� ]--------------------------------*/
	void IRgen(Tree* p, const char* output) { 
		fout = fopen(output, "w+");
		walkTree(p);
		fclose(fout);
	}
	/*--------------------------------[ ������ ]--------------------------------*/
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
