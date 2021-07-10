#include <stdio.h>

#include "lval.h"

lval lval_num(long num) {
    lval val;
    val.err = LNOERR;
    val.num = num;
    return val;
}

lval lval_err(int x) {
    lval val;
    val.err = x;
    return val;
}

void lval_print(lval val) {
    switch (val.err) {
        case LNOERR: 
            printf("%li", val.num);
            break;
        case LERR_DIV_ZERO:
            printf("error: divide by zero");
            break;
        case LERR_BAD_OP:
            printf("error: bad operator");
            break;
        case LERR_BAD_NUM:
            printf("error: invalid number");
            break;
        default:
            printf("unknown state");
    }
}

void lval_println(lval val) {
    lval_print(val);
    putchar('\n');
}

