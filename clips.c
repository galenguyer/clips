#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <editline/readline.h>
#include <histedit.h>

#include "mpc/mpc.h"

long eval_op(long left, char* operator, long right) {
    if (strcmp(operator, "+") == 0) return left + right;
    if (strcmp(operator, "-") == 0) return left - right;
    if (strcmp(operator, "*") == 0) return left * right;
    if (strcmp(operator, "/") == 0) return left / right;
    if (strcmp(operator, "%") == 0) return left % right;
    if (strcmp(operator, "^") == 0) return pow(left, right);
    return 0;
}

long eval(mpc_ast_t* t) {
    if (strstr(t->tag, "number")) {
        return atoi(t->contents);
    }

    char* op = t->children[1]->contents;

    long result = eval(t->children[2]);

    int pos = 3;
    while (strstr(t->children[pos]->tag, "expr")) {
        result = eval_op(result, op, eval(t->children[pos]));
        pos++;
    }
    return result;
}

int main(int argc, char** argv) {
    puts("clips!");
    puts("soon to be a small lisp, written in c");

    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Clips = mpc_new("clips");

    mpca_lang(MPCA_LANG_DEFAULT, 
        "                                                           \
            number      : /-?[0-9]+/ ;                              \
            operator    : '+' | '-' | '*' | '/' | '%' | '^' ;      \
            expr        : <number> | '(' <operator> <expr>+ ')' ;   \
            clips       : /^/ <operator> <expr>+ /$/ ;              \
        ", 
        Number, Operator, Expr, Clips);

    while (1) {
        char* input = readline("clips> ");
        add_history(input);

        mpc_result_t tree;
        if (mpc_parse("<stdin>", input, Clips, &tree)) {
            //mpc_ast_print(result.output);
            long result = eval(tree.output);
            printf("%li\n", result);
            mpc_ast_delete(tree.output);
        } else {
            mpc_err_print(tree.error);
            mpc_err_delete(tree.error);
        }

        free(input);
    }

    mpc_cleanup(4, Number, Operator, Expr, Clips);
    return 0;
}
