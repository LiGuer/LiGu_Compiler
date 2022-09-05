#ifndef CALCULATOR_ERROR_H
#define CALCULATOR_ERROR_H

#include <stdlib.h>

/*
 *  Error
 */

void expect(char t, char a) {
    if (t != a)
        exit(1);
    return;
}

#endif