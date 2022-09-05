#include <stdlib.h>
#include <stdio.h>
#include "token.h"
#include "lex.h"
#include "parse.h"
#include "error.h"


char* pos;
int t;

/*
 *  main
 */
int main(int argc, char* argv[]) {

}

double calculator(char* code) {
    double ans = 0;

    pos = &code[0];
    t = lex();
    ans = exprBinary(4);
    
    return ans;
}

