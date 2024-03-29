/***************************************************************
*			符号表
*	Symbol	Value	String	prec
*	符号	标记值	名称	优先级
----------------------------------------------------------------
[48 - 57]: 为数字 '0'-'9'
[65 - 90]: 为大写 'A' - 'Z'
[97 - 122]: 为小写 'a' - 'z'
"\'"[39],"\""[34],"\\"[92],"_"[95],"`"[96],
" "[32],"#"[35],"$"[36],"@"[64]
因为在词法分析器lex(,	0	)中，
数字被分析为[ NUM ] , [ REAL ]类型，
大小写字母被分析为[ ID ] , 及相应的指令，
引号字符串被分析为[ STR ]类型，
故数字 , 大小写字母在这张表里毫无用处，所以被删去。
数字[48 - 57]被征用为 "更复杂的运算符"，
大写[65 - 90]被征用为 "具体的字母指令"
小写[97 - 122]被空出，以后需要再用。
----------------------------------------------------------------
**prec[]是运算符优先级，只针对二元运算符，
prec=3级被空出，prec[","] = 1 , prec["="] = 2,
prec范围在[1,13]
***************************************************************/
xx(	NUL		,	0	,	"0"	,	0	,	0	,	0	,	0	)// 0
xx(	BOOL	,	1	,	"bool"	,	0	,	0	,"INTEG"	)//声明
xx(	CHAR	,	2	,	"char"	,	0	,	0	,"INTEG"	)
xx(	SHORT	,	3	,	"short"	,	0	,	0	,"INTEG"	)
xx(	INT		,	4	,	"int"	,	0	,	0	,"INTEG"	)
xx(	INT64	,	5	,	"int64"	,	0	,	0	,"INTEG"	)
xx(	FLOAT	,	6	,	"float"	,	0	,	0	,"FLOAT"	)
xx( FLOAT64	,	7	,	"float64",	0	,	0	,"FLOAT"	)
xx(	CLASS	,	8	,	"class",	0	,	0	,	0	)
xx(	ENUM	,	9	,	"enum"	,	0	,	0	,	0	)
xx(	VOID	,	10	,	"void"	,	0	,	0	,	0	)
xx(	UNSIGNED,	11	,	"unsigned",	0	,	0	,	0	)
xx(	CONST	,	12	,	"const"	,	0	,	0	,	0	)
xx( STATIC	,	13	,	"static",	0	,	0	,	0	)
xx(	EXTERN	,	14	,	"extern",	0	,	0	,	0	)
yy(	0	,	15	,	0	,	0	,	0	,	0	)
yy(	0	,	16	,	0	,	0	,	0	,	0	)
yy(	0	,	17	,	0	,	0	,	0	,	0	)
yy(	0	,	18	,	0	,	0	,	0	,	0	)
xx(	NUM		,	19	,	"num"	,	0	,	0	,	0	)//数值类型
xx(	REAL	,	20	,	"real"	,	0	,	0	,	0	)
xx(	STR		,	21	,	"str"	,	0	,	0	,	0	)
xx(	ID		,	22	,	"id"	,	0	,	0	,	0	)
yy(	0	,	23	,	0	,	0	,	0	,	0	)
yy(	0	,	24	,	0	,	0	,	0	,	0	)
yy(	0	,	25	,	0	,	0	,	0	,	0	)
yy(	0	,	26	,	0	,	0	,	0	,	0	)
yy(	0	,	27	,	0	,	0	,	0	,	0	)
yy(	0	,	28	,	0	,	0	,	0	,	0	)
yy(	0	,	29	,	0	,	0	,	0	,	0	)
yy(	0	,	30	,	0	,	0	,	0	,	0	)
yy(	0	,	31	,	0	,	0	,	0	,	0	)
yy(	0	,	32	,	0	,	0	,	0	,	0	)
yy(	0	,	33	,	"!"	,	0	,	0	,"LOGNOT")//运算符
yy(	0	,	34	,	0	,	0	,	0	,	0	)
yy(	0	,	35	,	0	,	0	,	0	,	0	)
yy(	0	,	36	,	0	,	0	,	0	,	0	)
yy(	0	,	37	,	"%"	,	13	,	0	,"MOD"	)
yy(	0	,	38	,	"&"	,	8	,	0	,"AND"	)
yy(	0	,	39	,	0	,	0	,	0	,	0	)
yy(	0	,	40	,	"("	,	0	,	0	,	0	)
yy(	0	,	41	,	")"	,	0	,	0	,	0	)
yy(	0	,	42	,	"*"	,	13	,	0	,"MUL"	)
yy(	0	,	43	,	"+"	,	12	,	0	,"ADD"	)
yy(	0	,	44	,	","	,	1	,	0	,	0	)
yy(	0	,	45	,	"-"	,	12	,	0	,"SUB"	)
yy(	0	,	46	,	"."	,	0	,	0	,	0	)
yy(	0	,	47	,	"/"	,	13	,	0	,"DIV"	)
xx(	ADDONE	,	48	,	"++"	,	0	,	0	,	0	)
xx(	SUBONE	,	49	,	"--"	,	0	,	0	,	0	)
xx(	ANDAND	,	50	,	"&&"	,	5	,	0	,"LOGAND")
xx(	OROR	,	51	,	"||"	,	4	,	0	,"LOGOR ")
xx(	LEQ		,	52	,	"<="	,	10	,	0	,"LEC"	)
xx(	GEQ		,	53	,	">="	,	10	,	0	,"GEC"	)
xx(	EQL		,	54	,	"=="	,	9	,	0	,"ECM"	)
xx(	NQL		,	55	,	"!="	,	9	,	0	,"NCM"	)
xx(	LSHIFT	,	56	,	"<<"	,	11	,	0	,"LSH"	)
xx(	RSHIFT	,	57	,	">>"	,	11	,	0	,"RSH"	)
yy(	0	,	58	,	":"	,	0	,	0	,	0	)
yy(	0	,	59	,	";"	,	0	,	0	,	0	)
yy(	0	,	60	,	"<"	,	10	,	0	,"LCM"	)
yy(	0	,	61	,	"="	,	2	,	0	,"STORE")
yy(	0	,	62	,	">"	,	10	,	0	,"GCM"	)
yy(	0	,	63	,	"?"	,	0	,	0	,	0	)
yy(	0	,	64	,	0	,	0	,	0	,	0	)
xx(	BREAK	,	65	,	"break"	,	0	,	0	,	0	)//控制语句
xx(	CASE	,	66	,	"case"	,	0	,	0	,	0	)
xx(	CONTINUE,	67	,	"continue",	0	,	0	,	0	)
xx(	DEFAULT	,	68	,	"default",	0	,	0	,	0	)
xx(	ELSE	,	69	,	"else"	,	0	,	0	,	0	)
xx(	FOR		,	70	,	"for"	,	0	,	0	,	0	)
xx(	GOTO	,	71	,	"goto"	,	0	,	0	,"JMP"	)
xx(	IF		,	72	,	"if"	,	0	,	0	,	0	)
xx(	RETURN	,	73	,	"return",	0	,	0	,	0	)
xx(	SWITCH	,	74	,	"switch",	0	,	0	,	0	)
xx(	WHILE	,	75	,	"while"	,	0	,	0	,	0	)
xx(	SIZEOF	,	76	,	"sizeof",	0	,	0	,	0	)
yy(	0	,	77	,	0	,	0	,	0	,	0	)
yy(	0	,	78	,	0	,	0	,	0	,	0	)
yy(	0	,	79	,	0	,	0	,	0	,	0	)
yy(	0	,	80	,	0	,	0	,	0	,	0	)
yy(	0	,	81	,	0	,	0	,	0	,	0	)
yy(	0	,	82	,	0	,	0	,	0	,	0	)
yy(	0	,	83	,	0	,	0	,	0	,	0	)
yy(	0	,	84	,	0	,	0	,	0	,	0	)
yy(	0	,	85	,	0	,	0	,	0	,	0	)
yy(	0	,	86	,	0	,	0	,	0	,	0	)
yy(	0	,	87	,	0	,	0	,	0	,	0	)
yy(	0	,	88	,	0	,	0	,	0	,	0	)
yy(	0	,	89	,	0	,	0	,	0	,	0	)
yy(	0	,	90	,	0	,	0	,	0	,	0	)
yy(	0	,	91	,	"["	,	0	,	0	,	0	)
yy(	0	,	92	,	0	,	0	,	0	,	0	)
yy(	0	,	93	,	"]"	,	0	,	0	,	0	)
yy(	0	,	94	,	"^"	,	7	,	0	,	0	)
yy(	0	,	95	,	0	,	0	,	0	,	0	)
yy(	0	,	96	,	0	,	0	,	0	,	0	)
yy(	0	,	97	,	0	,	0	,	0	,	0	)
yy(	0	,	98	,	0	,	0	,	0	,	0	)
yy(	0	,	99	,	0	,	0	,	0	,	0	)
yy(	0	,	100	,	0	,	0	,	0	,	0	)
yy(	0	,	101	,	0	,	0	,	0	,	0	)
yy(	0	,	102	,	0	,	0	,	0	,	0	)
yy(	0	,	103	,	0	,	0	,	0	,	0	)
yy(	0	,	104	,	0	,	0	,	0	,	0	)
yy(	0	,	105	,	0	,	0	,	0	,	0	)
yy(	0	,	106	,	0	,	0	,	0	,	0	)
yy(	0	,	107	,	0	,	0	,	0	,	0	)
yy(	0	,	108	,	0	,	0	,	0	,	0	)
yy(	0	,	109	,	0	,	0	,	0	,	0	)
yy(	0	,	110	,	0	,	0	,	0	,	0	)
yy(	0	,	111	,	0	,	0	,	0	,	0	)
yy(	0	,	112	,	0	,	0	,	0	,	0	)
yy(	0	,	113	,	0	,	0	,	0	,	0	)
yy(	0	,	114	,	0	,	0	,	0	,	0	)
yy(	0	,	115	,	0	,	0	,	0	,	0	)
yy(	0	,	116	,	0	,	0	,	0	,	0	)
yy(	0	,	117	,	0	,	0	,	0	,	0	)
yy(	0	,	118	,	0	,	0	,	0	,	0	)
yy(	0	,	119	,	0	,	0	,	0	,	0	)
yy(	0	,	120	,	0	,	0	,	0	,	0	)
yy(	0	,	121	,	0	,	0	,	0	,	0	)
yy(	0	,	122	,	0	,	0	,	0	,	0	)
yy(	0	,	123	,	"{"	,	0	,	0	,	0	)
yy(	0	,	124	,	"|"	,	6	,	0	,"OR "	)
yy(	0	,	125	,	"}"	,	0	,	0	,	0	)
yy(	0	,	126	,	"~"	,	0	,	0	,"NOT"	)
xx(	EOI	,	127	,	"end"	,	0	,	0	,	0	)//结尾
#undef xx
#undef yy