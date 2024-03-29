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
#include "compiler.h"
#include <string.h>

char* init(char url[]) {
	FILE* fp = fopen(url, "r");
	// File Size
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	rewind(fp);
	// Save File's Code chars
	char* codePosSource = (char*)malloc(sizeof(char) * (size + 1));
	memset(codePosSource, 0, sizeof(char) * (size + 1));
	fread(codePosSource, size, 1, (FILE*)fp);
	codePosSource[size] = EOI;
	fclose(fp);
	return codePosSource;
}

int ArgPos(char* str, int argc, char** argv) {
	for (int a = 1; a < argc; a++) if (!strcmp(str, argv[a])) {
		if (a == argc - 1) { printf("Argument missing for %s\n", str); exit(-1); }
		return a;
	} return -1;
}

int main(int argc, char* argv[]) {
	char inputUrl[1000] = "D:/LiGu_Codes/LiGu_Compiler/test/test.c", outputUrl[1000]= "D:/LiGu_Codes/LiGu_Compiler/test/test.ll";
	/*
	// input
	if (argc == 1) {
		printf("LiGu_C-Compiler\n\nOptions:\n");
		printf("\t-InputUrl <file>\n");
		printf("\t-OutputUrl <file>\n");
		return 0;
	}
	int i = 0;
	if ((i = ArgPos((char*)"-InputUrl", argc, argv)) > 0) strcpy(inputUrl, argv[i + 1]);
	if ((i = ArgPos((char*)"-OutputUrl", argc, argv)) > 0) strcpy(outputUrl, argv[i + 1]);
	// begin*/
	Compiler compiler(init(inputUrl));
	compiler.compiler(outputUrl);
}

