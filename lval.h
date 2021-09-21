#ifndef __LVAL
#define __LVAL
#include "mpc/mpc.h"

typedef struct lval {
    int type;
    // LVAL_ERR
    char* err;
    // LVAL_NUM
    long num;
    // LVAL_SYM
    char* sym;
    // LVAL_SEXPR
    int count;
    struct lval** cells;
} lval;

enum {
    LVAL_ERR,
    LVAL_NUM,
    LVAL_SYM,
    LVAL_SEXPR
};

lval* lval_read(mpc_ast_t* ast);
lval* lval_num(long num);
lval* lval_err(char* err);
lval* lval_sym(char* sym);
lval* lval_sexpr();
void lval_del(lval* val);
void lval_print(lval* val);
void lval_println(lval* val);
#endif