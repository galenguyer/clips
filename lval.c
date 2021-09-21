#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lval.h"
#include "mpc/mpc.h"

lval* lval_num(long num) {
    lval* val = (lval*) malloc(sizeof(lval));
    val->type = LVAL_NUM;
    val->num = num;
    return val;
}

lval* lval_err(char* err) {
    lval* val = (lval*) malloc(sizeof(lval));
    val->type = LVAL_ERR;
    val->err = malloc(strlen(err) + 1);
    strncpy(val->err, err, strlen(err));
    return val;
}

lval* lval_sym(char* sym) {
    lval* val = (lval*) malloc(sizeof(lval));
    val->type = LVAL_SYM;
    val->sym = malloc(strlen(sym) + 1);
    strncpy(val->sym, sym, strlen(sym));
    val->sym[strlen(sym)] = '\0';
    return val;
}

lval* lval_sexpr() {
    lval* val = (lval*) malloc(sizeof(lval));
    val->type = LVAL_SEXPR;
    val->count = 0;
    val->cells = NULL;
    return val;
}

lval* lval_read_num(mpc_ast_t* ast) {
    // reset errno
    errno = 0;
    long x = strtol(ast->contents, NULL, 10);
    return errno == 0 ? lval_num(x) : lval_err("invalid number");
}

lval* lval_add(lval* parent, lval* cell) {
    parent->count++;
    parent->cells = realloc(parent->cells, sizeof(lval) * parent->count);
    parent->cells[parent->count - 1] = cell;
    return parent;
}

lval* lval_read(mpc_ast_t* ast) {
    if (strstr(ast->tag, "number")) {
        return lval_read_num(ast);
    }
    if (strstr(ast->tag, "symbol")) {
        return lval_sym(ast->contents);
    }

    lval* val = NULL;
    // create a new list if this is the root node
    if (strcmp(ast->tag, ">") == 0) {
        val = lval_sexpr();
    }
    if (strstr(ast->tag, "sexpr")) {
        val = lval_sexpr();
    }

    for (int i = 0; i < ast->children_num; i++) {
        if (strcmp(ast->children[i]->contents, "(") == 0) {
            continue;
        }
        if (strcmp(ast->children[i]->contents, ")") == 0) {
            continue;
        }
        if (strcmp(ast->children[i]->tag,  "regex") == 0) {
            continue;
        }
        val = lval_add(val, lval_read(ast->children[i]));
    }

    return val;
}

void lval_del(lval* val) {
    switch (val->type) {
        case LVAL_NUM:
            break;
        case LVAL_ERR:
            free(val->err);
            break;
        case LVAL_SYM:
            free(val->sym);
            break;
        case LVAL_SEXPR:
            for (int i = 0; i < val->count; i++) {
                lval_del(val->cells[i]);
            }
            free(val->cells);
            break;
    }
    free(val);
}

void lval_print_expr(lval* val, char open, char close) {
    printf("%c", open);

    for (int i = 0; i < val->count; i++) {
        lval_print(val->cells[i]);

        if (i != val->count-1) {
            printf(" ");
        }
    }

    printf("%c", close);
}

void lval_print(lval* val) {
    switch (val->type) {
        case LVAL_NUM:
            printf("%li", val->num);
            break;
        case LVAL_ERR:
            printf("error: %s", val->err);
            break;
        case LVAL_SYM:
            printf("%s", val->sym);
            break;
        case LVAL_SEXPR:
            lval_print_expr(val, '(', ')');
            break;
        default:
            printf("unknown type");
    }
}

void lval_println(lval* val) {
    lval_print(val);
    putchar('\n');
}
