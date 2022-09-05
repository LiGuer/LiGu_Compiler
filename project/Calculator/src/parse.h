#ifndef CALCULATOR_PARSE_H
#define CALCULATOR_PARSE_H

#include "lex.h"
#include "error.h"

/*
 *  Parse, Syntax Analysis
 */

double statements() {

}

double assign() {

}

double expr(int level) {

}

double expr_0() {
    double p;

    switch (t) {
    case NUM: p = u.i; break;
    case REAL:p = u.d; break;
    case VAR:p = u.d; break;
    case '(':
        t = lex();
        p = exprBinary(4);
        expect(t, ')');
        break;
    }

    t = lex();
    return p;
}








#endif