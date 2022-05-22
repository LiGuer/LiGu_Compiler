import re

def Formulas(Str):
	# Greek 字母
	Greek = ['α', 'β', 'γ', 'δ', 'ε', 'ζ', 'η', 'θ', 'ι', 'κ', 'λ', 'μ', 'ν', 'ξ', 'ο', 'π', 'ρ', 'σ', 'τ', 'υ', 'φ', 'χ', 'ψ', 'ω', 'Γ', 'Δ', 'Θ', 'Λ', 'Ξ', '∏', 'Σ', 'Φ','Ψ', 'Ω' ]
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

	# Other
	Str = re.sub(r'\\l ', r'\\left ', Str)
	Str = re.sub(r'\\r ', r'\\right ', Str)
	Str = re.sub(r'\\sum_', r'\\sum\\limits_', Str)
	Str = re.sub(r'\\prod_', r'\\prod\\limits_', Str)
	
	Str = re.sub(r'\\d ', r'\\mathrm d ', Str)
	Str = re.sub(r'\\/', r'\\frac', Str)
	
	Str = re.sub(r'\\qu', r'\\quad', Str)
	Str = re.sub(r'\\quadad', r'\\quad', Str)
	Str = re.sub(r'\\bb', r'\\matgbb', Str)
	# $
	Str = re.sub(r'\$\s*\n', r'$\n', Str)
	while(1):
		# begin
		ans = re.search('\n\s*\$', Str)
		if (ans == None): 
			break
		a = ans.span()[1]
		StrA = Str[:a-1] + "\\begin{align*}"
		Str = Str[a:]
		# end
		ans = re.search('\$\n', Str)
		if (ans == None): 
			break
		c = ans.span()[1]
		StrC = "\\end{align*}\n" + Str[c:]
		# middle
		StrB = Str[:c-2]
		StrB = re.sub(r'\n', r'\\\\\n', StrB)

		StrB = re.sub(r'\(', r'\\left(', StrB)
		StrB = re.sub(r'\)', r'\\right)', StrB)
		StrB = re.sub(r'\[', r'\\left[', StrB)
		StrB = re.sub(r'\]', r'\\right]', StrB)
		StrB = re.sub(r'\\\{', r'\\left\\{', StrB)
		StrB = re.sub(r'\\\}', r'\\right\\}', StrB)

		StrB = re.sub(r'(\S)\/(\S)', lambda m: '\\frac{' + m.group(1) + '}{' + m.group(2) + '}', StrB)

		StrB = re.sub(r'\\mb', r'\\begin{matrix}', StrB)
		StrB = re.sub(r'\\me', r'\\end{matrix}', StrB)
		# ans
		Str = StrA + StrB + StrC

	Str = re.sub(r'\n\\\\\n', r'\n', Str)
	Str = re.sub(r'\\\\\\\\', r'\\\\', Str)
	return Str