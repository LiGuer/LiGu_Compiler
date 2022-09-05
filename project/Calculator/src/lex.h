#ifndef CALCULATOR_LEX_H
#define CALCULATOR_LEX_H

#include "token.h"
#include "error.h"

/*
 *  Lexical Analysis
 */

int lex();
int lex_Int();
int lex_Real();


int lex() {
    while (1) {
        switch (*pos++) {
        case '0':case '1':case '2':case '3':case '4':
        case '5':case '6':case '7':case '8':case '9':
            return lex_Int();

        case '+':case '-':case '=':case '&':case '|':
        case '*':case '/':case '^':case '%':
        case '(':case ')':case '[': case ']': case '{': case '}':
            return pos[-1];
        default:
            continue;
        }
    }
}

int lex_Int() {
    char* tmp = pos - 1;
    int num = 0;

    while (*tmp >= '0' && *tmp <= '9') {
        num = num * 10 + *tmp - '0';
        tmp++;
    }

    if (*tmp == '.' || *tmp == 'e' || *tmp == 'E')
        return lex_Real();

    pos = tmp;
    u.i = num;
    return NUM;
}


int lex_Real() {
    char* tmp = pos - 1;
    double real = 0;

    while (*tmp >= '0' && *tmp <= '9') {
        real = real * 10 + *tmp - '0';
        tmp++;
    }

    if (*tmp == '.') {
        tmp++; 
        double cur = 0.1;

        while (*tmp >= '0' && *tmp <= '9') {
            real += (*tmp - '0') * cur;
            cur *= 0.1;
            tmp++;
        }
    }
    else if (*tmp == 'e' || *tmp == 'E') {
        tmp++; 
        double cur = 1; 
        int flag = 1;

        if (*tmp == '-') { 
            flag = -1; 
            tmp++; 
        }
        else if (*tmp == '+') 
            tmp++;

        while (*tmp >= '0' && *tmp <= '9') {
            int i = *tmp - '0';

            while (i--)
                cur *= 10;

            tmp++;
        }

        if (flag == -1)
            cur = 1 / cur;
        real *= cur;
    }

    pos = tmp;
    u.d = real;
    return REAL;
}



#endif