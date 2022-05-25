import re

def Formulas(Str):

	# Greek 字母
	Greek = [
		'α', 'β', 'γ', 'δ', 'ε', 'ζ', 'η', 'θ', 'ι', 'κ', 'λ', 'μ', 'ν', 'ξ', 'ο', 'π', 'ρ', 'σ', 'τ', 'υ', 'φ', 'χ', 'ψ', 'ω', 
		'Γ', 'Δ', 'Θ', 'Λ', 'Ξ', '∏', 'Σ', 'Φ','Ψ', 'Ω' 
	]
	GreekLatex = [
		r'\\alpha ', r'\\beta ', r'\\gamma ', r'\\delta ', r'\\epsilon ', r'\\zeta ', r'\\eta ', r'\\theta ', r'\\iota ', r'\\kappa ', r'\\lambda ', r'\\mu ', r'\\nu ', r'\\xi ', r'\\omicron ', r'\\pi ', r'\\rho ', r'\\sigma ', r'\\tau ', r'\\upsilon ', r'\\phi ', r'\\chi ', r'\\psi ', r'\\omega ', 
		r'\\Gamma ', r'\\Delta ', r'\\Theta ', r'\\Lambda ', r'\\Xi ', r'\\Pi ', r'\\Sigma ', r'\\Phi',r'\\Psi ', r'\\Omega '
	]

	for i in range(0, len(Greek)):
		Str = re.sub(Greek[i], GreekLatex[i], Str)

	# 数学符号
	MathSymbol = [
		'≥', '≤', '≠', '⪰', '⪯',
		'×', '~', '∂', '∇', '\|\|', '%', 
		' => ', '\t=> ', ' <=> ', 
		r'\\\.'
	]
	MathSymbolLatex = [
		r'\\ge ', r'\\le ', r'\\neq ', r'\\succeq ', r'\\preceq ',
		r'\\times ', r'\\sim ', r'\\partial ', r'\\nabla ', r'\\| ', r'\\% ', 
		r' \\Rightarrow ', r'\t\\Rightarrow ', r' \\Leftrightarrow ',
		r'\\boldsymbol '
		]

	for i in range(0, len(MathSymbol)):
		Str = re.sub(MathSymbol[i], MathSymbolLatex[i], Str)


	# 括号
	Str = re.sub(r'\(', r'\\left(', Str)
	Str = re.sub(r'\)', r'\\right)', Str)
	Str = re.sub(r'\[', r'\\left[', Str)
	Str = re.sub(r'\]', r'\\right]', Str)
	Str = re.sub(r'\\\{', r'\\left\\{', Str)
	Str = re.sub(r'\\\}', r'\\right\\}', Str)

	Str = re.sub(r'\\mb', r'\\begin{matrix}', Str)
	Str = re.sub(r'\\me', r'\\end{matrix}', Str)

	# Other
	Str = re.sub(r'\n', r'\\\\\n', Str)
	Str = re.sub(r'\\begin\{matrix\}\\\\\n', r'\\begin{matrix}\n', Str)

	Str = re.sub(r'(\S)\/(\S)', lambda m: '\\frac{' + m.group(1) + '}{' + m.group(2) + '}', Str)

	Str = re.sub(r'\\l ', r'\\left ', Str)
	Str = re.sub(r'\\r ', r'\\right ', Str)
	Str = re.sub(r'\\sum_', r'\\sum\\limits_', Str)
	Str = re.sub(r'\\prod_', r'\\prod\\limits_', Str)
	
	Str = re.sub(r'\\d ', r'\\mathrm d ', Str)
	Str = re.sub(r'\\/', r'\\frac', Str)
	
	Str = re.sub(r'\\qu', r'\\quad', Str)
	Str = re.sub(r'\\quadad', r'\\quad', Str)
	Str = re.sub(r'\\bb', r'\\mathbb', Str)

	return Str



def FormulasInterLine(Str):
	Str = Formulas(Str)
	Str = re.sub(r'\$', r'\\begin{align*}', Str, count=1)
	Str = re.sub(r'\$\\\\', r'\\end{align*}\n', Str)

	return Str



def FormulasInLine(Str):
	Str = Formulas(Str)
	
	return Str



# Detect Formulas
def DetectFormulas(Str):
	# Inter Line Formulas
	InterLineFormulas = []
	ind = 0

	while(1):
		# begin
		tmp = re.search('\n\s*\$', Str)
		if (tmp == None): 
			break

		a = tmp.span()[0] + 1
		b = tmp.span()[1]

		StrA = Str[b:]

		#end
		tmp = re.search('\$\n', StrA)
		c = tmp.span()[0]
		d = tmp.span()[1]

		formulas = Str[b-2:d + b + 1]
		formulas = re.sub(r'\t', '', formulas)
		InterLineFormulas = InterLineFormulas + [formulas]

		Str = Str[:b-2] + "@(InterLineFormulas_" + str(ind) +"_InterLineFormulas)@\n" + Str[d+b+1:]
		ind = ind + 1

	# In Line Formulas
	InLineFormulas = []
	ind = 0

	while(1):
		# begin
		tmp = re.search('\$', Str)
		if (tmp == None): 
			break

		a = tmp.span()[0]

		StrA = Str[a+1:]

		#end
		tmp = re.search('\$', StrA)
		c = tmp.span()[0]

		formulas = Str[a:a + c + 2]
		InLineFormulas = InLineFormulas + [formulas]

		Str = Str[:a] + "@(InLineFormulas_" + str(ind) +"_InLineFormulas)@" + Str[a + c + 2:]
		ind = ind + 1

	return [Str, InterLineFormulas, InLineFormulas]