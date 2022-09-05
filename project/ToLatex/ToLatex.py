import re
import argparse
import math
import os
import random
import sys
from Formulas import *
from Codes import *

# 简单命令
def EasyCode(Str):
	Str = re.sub(r' -- ', r' --- ', Str)
	Str = re.sub(r'\\bf', r'\\textbf', Str)

	Marks = [
		'Define', 'Property',
		'Problem', 'Algorithm', 'Situation',
		'Include', 'Example', 
		'Proof', 'Theorem', 'Note'
	]

	for i in range(0, len(Marks)):
		Str = re.sub(r'\\' + Marks[i] + r'\{([\S]+)\}', lambda m: '\\textbf{' + Marks[i] + ' (' + m.group(1)+ '). }', Str)
		Str = re.sub(r'\\' + Marks[i], r'\\textbf{' + Marks[i] + r'. }', Str)

	return Str

def Item(Str):
	while(1):
		# a
		ans = re.search(r'\n(\s*)\*', Str)
		if (ans == None): 
			break
		a = ans.span()[0]
		b = ans.span()[1]
		n = 0
		for i in range(a, b):
			if(Str[i] == '\t'):
				n = n + 1
		# begin
		StrA = Str[:a] + '\n' +  '\t' * n + "\\begin{enumerate}"
		Str = Str[a:]
		# c-1
		strTmp = r'\n\t{0,' + str(int(n-1)) + r'}\S'
		ans = re.search(strTmp, Str)
		if (ans == None): 
			c = len(Str) - 1
		else:
			c = ans.span()[0]
		# c-2
		strTmp = r'\n\t{0,' + str(int(n)) + r'}[^\*\t]'
		ans = re.search(strTmp, Str)
		if (ans != None): 
			c = min(c, ans.span()[0])
		# end
		StrC = Str[:c+1] + '\t' * n + "\\end{enumerate}\n"
		# ans
		strTmp = r'(\n\t{' + str(int(n)) + r'})\* '
		StrC = re.sub(strTmp, lambda m:m.group(1) + "\\item " , StrC)
		Str = StrA + StrC + Str[c+1:]

	while(1):
		# a
		ans = re.search(r'\n(\s*)\-', Str)
		if (ans == None): 
			break
		a = ans.span()[0]
		b = ans.span()[1]
		n = 0
		for i in range(a, b):
			if(Str[i] == '\t'):
				n = n + 1
		# begin
		StrA = Str[:a] + '\n' +  '\t' * n + "\\begin{itemize}"
		Str = Str[a:]
		# c-1
		strTmp = r'\n\t{0,' + str(int(n-1)) + r'}\S'
		ans = re.search(strTmp, Str)
		if (ans == None): 
			c = len(Str) - 1
		else:
			c = ans.span()[0]
		# c-2
		strTmp = r'\n\t{0,' + str(int(n)) + r'}[^\-\t]'
		ans = re.search(strTmp, Str)
		if (ans != None): 
			c = min(c, ans.span()[0])
		# end
		StrC = Str[:c+1] + '\t' * n + "\\end{itemize}\n"
		# ans
		strTmp = r'(\n\t{' + str(int(n)) + r'})\- '
		StrC = re.sub(strTmp, lambda m:m.group(1) + "\\item " , StrC)
		Str = StrA + StrC + Str[c+1:]
	return Str

# 章节标题
def Section(Str):
	Str = re.sub(r'\n\*', r'\n\\section', Str)
	Str = re.sub(r'(\n\t{1})\*', lambda m:m.group(1) +'\\subsection', Str)
	Str = re.sub(r'(\n\t{2})\*', lambda m:m.group(1) +'\\subsubsection', Str)
	Str = re.sub(r'section ([\s\S][^\n]+)\n', lambda m:'section{' + m.group(1) + '}\n', Str)
	return Str

# txt -> Latex 函数
def ToLatex(Str):
	# 公式
	[Str, InterLineFormulas, InLineFormulas] = DetectFormulas(Str)

	for i in range(0, len(InterLineFormulas)):
		InterLineFormulas[i] = FormulasInterLine(InterLineFormulas[i])

	for i in range(0, len(InLineFormulas)):
		InLineFormulas[i] = FormulasInLine(InLineFormulas[i])

	# 代码
	[Str, CodesSet] = DetectCodes(Str)

	for i in range(0, len(CodesSet)):
		CodesSet[i] = Codes(CodesSet[i])

	# 简单命令
	Str = '\n' + Str + '\n' 
	Str = re.sub(r'\n\s*\n', r'\n', Str)
	
	Str = EasyCode(Str)		# 简单命令
	# Str = Section(Str)		# 章节标题
	Str = Item(Str)		# 列表

	# Other
	Str = re.sub(r'\n', r'\\par\n', Str)
	Str = re.sub(r'\\begin{itemize}\\par', r'\\begin{itemize}', Str)
	Str = re.sub(r'\\end{itemize}\\par', r'\\end{itemize}', Str)

	# 公式、代码 插回

	while(1):
		tmp = re.search(r'@\(InterLineFormulas_', Str)
		if (tmp == None): 
			break

		a = tmp.span()[0]
		b = tmp.span()[1]

		tmp = re.search(r'_InterLineFormulas\)@', Str)
		c = tmp.span()[0]
		d = tmp.span()[1]

		Str = Str[:a] + InterLineFormulas[int(Str[b:c])] + Str[d:]


	while(1):
		tmp = re.search(r'@\(InLineFormulas_', Str)
		if (tmp == None): 
			break

		a = tmp.span()[0]
		b = tmp.span()[1]

		tmp = re.search(r'_InLineFormulas\)@', Str)
		c = tmp.span()[0]
		d = tmp.span()[1]

		Str = Str[:a] + InLineFormulas[int(Str[b:c])] + Str[d:]


	while(1):
		tmp = re.search(r'@\(Codes_', Str)
		if (tmp == None): 
			break

		a = tmp.span()[0]
		b = tmp.span()[1]

		tmp = re.search(r'_Codes\)@', Str)
		c = tmp.span()[0]
		d = tmp.span()[1]

		Str = Str[:a] + CodesSet[int(Str[b:c])] + Str[d:]

	# 文件头
	file = open("head.tex","r", encoding='utf-8')
	Str = file.read() + Str + "\n\\end{document}"
	file.close()

	# 补丁
	Str = re.sub(r'\\end\{align\*\}\n\n\\par', r'\\end{align*}\n', Str)

	return Str

def ToLatex__(Str):
	Str = '\n' + Str + '\n' 
	Str = re.sub(r'\n\s*\n', r'\n', Str)
	
	Str = EasyCode(Str)		# 简单命令
	# Str = Section(Str)		# 章节标题
	Str = Item(Str)		# 列表
	Str = Formulas(Str)		# 公式
	Str = Codes(Str)		# 代码

	# Other
	Str = re.sub(r'\n', r'\\par\n', Str)
	Str = re.sub(r'@NOPAR\\par', r'', Str)
	Str = re.sub(r'\\\\\\par', r'\\\\', Str)
	Str = re.sub(r'\\begin{itemize}\\par', r'\\begin{itemize}', Str)
	Str = re.sub(r'\\end{itemize}\\par', r'\\end{itemize}', Str)
	
	Str = re.sub(r'\\begin{align\*}\\par', r'\\begin{align*}', Str)
	Str = re.sub(r'\\begin{align\*}\\\\', r'\\begin{align*}', Str)
	Str = re.sub(r'\\end{align\*}\\par', r'\\end{align*}', Str)
	Str = re.sub(r'\\par(\s+)\\begin{align\*}', lambda m: m.group(1) + '\\begin{align*}', Str)
	Str = re.sub(r'\\\\(\s+)\\end{align\*}', lambda m: m.group(1) + '\\end{align*}', Str)
	Str = re.sub(r'\\begin{matrix}\\\\', r'\\begin{matrix}', Str)

	file = open("head.tex","r", encoding='utf-8')
	Str = file.read() + Str + "\n\\end{document}"
	file.close()

	return Str

def parse_opt(known=False):
	parser = argparse.ArgumentParser()
	parser.add_argument('-i', type=str, default='')
	parser.add_argument('-o', type=str, default='')
	return parser.parse_known_args()[0] if known else parser.parse_args()

if __name__ == '__main__':
	opt = parse_opt()

	file = open(opt.i,"r", encoding='utf-8')
	Str = file.read()
	file.close()

	Str = ToLatex(Str)

	file = open(opt.o,"w+", encoding='utf-8')
	file.write(Str)
	file.close()