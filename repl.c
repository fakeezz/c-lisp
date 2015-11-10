
#include <mpc.h>
#include <editline/readline.h>
#include <editline/history.h>

#include "data.h"
#include "print.h"
#include "eval.h"

static char input[2048];

int main(int argc, char** argv) {

    puts("Lisp version 0.0.1\n");
    puts("Press Ctrl+c to Exit\n");


    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Symbol = mpc_new("symbol");
    mpc_parser_t* Sexpr = mpc_new("sexpr");
    mpc_parser_t* Qexpr = mpc_new("qexpr");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Lisp = mpc_new("lisp");

    mpca_lang(MPCA_LANG_DEFAULT,
    "                                                          \
        number   : /-?[0-9]+/;                                 \
        symbol   : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]+/ ;          \
        sexpr    : '(' <expr>* ')' ;                           \
        qexpr    : '{' <expr>* '}' ;                           \
        expr     : <number> | <symbol> | <sexpr> | <qexpr> ;   \
        lisp     : /^/ <expr>* /$/ ;                           \
    ",
    Number, Symbol, Sexpr, Qexpr, Expr, Lisp);


    lenv* e = lenv_new();
    lenv_add_builtins(e);

    while (1) {

        char* input = readline("lisp> ");
        add_history(input);

        mpc_result_t r;
        if (mpc_parse("<stdin>", input, Lisp, &r)) {
            lval* result = lval_eval(e, lval_read(r.output));
            lval_println(result);
            lval_del(result);
            mpc_ast_delete(r.output);
        } else {
            mpc_err_print(r.output);
            mpc_err_delete(r.output);
        }

        free(input);
    }

    mpc_cleanup(6, Number, Symbol, Sexpr, Qexpr, Expr, Lisp);
    lenv_del(e);

    return 0;
}
