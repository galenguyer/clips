#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <editline/readline.h>
#include <histedit.h>

#include "mpc/mpc.h"
#include "lval.h"

// lval eval_op(lval left, char* operator, lval right) {
//     if (left.err) return left;
//     if (right.err) return right;

//     if (strcmp(operator, "+") == 0) return lval_num(left.num + right.num);
//     if (strcmp(operator, "-") == 0) return lval_num(left.num - right.num);
//     if (strcmp(operator, "*") == 0) return lval_num(left.num * right.num);
//     if (strcmp(operator, "/") == 0) return right.num == 0 ? lval_err(LERR_DIV_ZERO) : lval_num(left.num / right.num);
//     if (strcmp(operator, "%") == 0) return lval_num(left.num % right.num);
//     if (strcmp(operator, "^") == 0) return lval_num(pow(left.num, right.num));

//     return lval_err(LERR_BAD_OP);
// }

// lval eval(mpc_ast_t* t) {
//     if (strstr(t->tag, "number")) {
//         errno = 0;
//         long x = strtol(t->contents, NULL, 10);
//         return errno == ERANGE ? lval_err(LERR_BAD_NUM) : lval_num(x);
//     }

//     char* op = t->children[1]->contents;

//     lval result = eval(t->children[2]);

//     int pos = 3;
//     while (strstr(t->children[pos]->tag, "expr")) {
//         result = eval_op(result, op, eval(t->children[pos]));
//         pos++;
//     }
//     return result;
// }

int main(int argc, char** argv) {
    puts("clips!");
    puts("soon to be a small lisp, written in c");

    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Symbol = mpc_new("symbol");
    mpc_parser_t* Sexpr = mpc_new("sexpr");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Clips = mpc_new("clips");

    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                           \
            number      : /-?[0-9]+/ ;                              \
            symbol      : '+' | '-' | '*' | '/' | '%' | '^' ;       \
            sexpr       : '(' <expr>* ')' ;                         \
            expr        : <number> | <symbol> | <sexpr> ;           \
            clips       : /^/ <expr>* /$/ ;                         \
        ",
        Number, Symbol, Sexpr, Expr, Clips);

    while (1) {
        char* input = readline("clips> ");
        if (input == NULL) {
            printf("\n");
            break;
        }
        add_history(input);

        mpc_result_t tree;
        if (mpc_parse("<stdin>", input, Clips, &tree)) {
            mpc_ast_print(tree.output);
            //lval result = eval(tree.output);
            lval* val = lval_read(tree.output);
            lval_println(val);
            lval_del(val);
            mpc_ast_delete(tree.output);
        } else {
            mpc_err_print(tree.error);
            mpc_err_delete(tree.error);
        }

        free(input);
    }

    mpc_cleanup(5, Number, Symbol, Sexpr, Expr, Clips);
    return 0;
}
