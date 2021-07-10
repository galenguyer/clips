typedef struct {
    long num;
    int err;
} lval;

enum {
    LNOERR,
    LERR_DIV_ZERO,
    LERR_BAD_OP,
    LERR_BAD_NUM
};

lval lval_num(long num);
lval lval_err(int x);
void lval_print(lval val);
void lval_println(lval val);
