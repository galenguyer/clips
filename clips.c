#include <stdio.h>
#include <stdlib.h>
#include <editline/readline.h>
#include <histedit.h>

#include "mpc/mpc.h"

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
            operator    : '+' | '-' | '*' | '/' ;                   \
            expr        : <number> | '(' <operator> <expr>+ ')' ;   \
            clips       : /^/ <operator> <expr>+ /$/ ;              \
        ", 
        Number, Operator, Expr, Clips);

    while (1) {
        char* input = readline("clips> ");
        add_history(input);

        mpc_result_t result;
        if (mpc_parse("<stdin>", input, Clips, &result)) {
            mpc_ast_print(result.output);
            mpc_ast_delete(result.output);
        } else {
            mpc_err_print(result.error);
            mpc_err_delete(result.error);
        }

        free(input);
    }

    mpc_cleanup(4, Number, Operator, Expr, Clips);
    return 0;
}
