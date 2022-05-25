import re

def Codes(Str):
	# a
	Str = re.sub(r'\\Code\{C\+\+\}\n', r"\\begin{lstlisting}\n",  Str)
	Str = Str + '\\end{lstlisting}\n'

	Str = re.sub(r'\t', "  ",  Str)

	return Str

# Detect Codes
def DetectCodes(Str):
	# Inter Line Formulas
	CodesSet = []
	ind = 0

	while(1):
		# begin
		tmp = re.search(r'\n\t*\\Code\{C\+\+\}\n', Str)
		if (tmp == None): 
			break

		a = tmp.span()[0] + 1
		b = tmp.span()[1]

		StrA = Str[b:]

		# tab number
		n = 0
		for i in range(a, b):
			if(Str[i] == '\t'):
				n = n + 1

		#end
		tmp = re.search(r'\n\t{0,' + str(int(n)) + r'}\S', StrA)
		c = tmp.span()[0]
		d = tmp.span()[1]

		code = Str[a+n:b + c + 1]

		code = re.sub(r'\n\t{' + str(int(n+1)) + r'}', "\n", code)
		CodesSet = CodesSet + [code]

		Str = Str[:a+n] + "@(Codes_" + str(ind) +"_Codes)@\n" + Str[b + c + 1:]
		ind = ind + 1


	return [Str, CodesSet]
