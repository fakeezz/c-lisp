
#include <mpc.h>
#include <editline/readline.h>

#include "types.h"
#include "print.h"
#include "ast.h"
#include "builtin.h"

#ifdef HAVE_PRELUDE
#include "stl.h"
#endif


static char input[2048];


void REPL(char* input, mpc_parser_t* parser, clisp_env_t* env, int loading) {
    mpc_result_t r;
    if (mpc_parse("<stdin>", input, parser, &r)) {
        clisp_expr_t* expr = clisp_read_ast(r.output);
        clisp_chunk_t* result = clisp_eval_ast(expr, env);
        if (loading == 0)
            clisp_print_writeln(result);
        clisp_chunk_delete(result);
        mpc_ast_delete(r.output);
    } else {
        mpc_err_print(r.output);
        mpc_err_delete(r.output);
    }
}


int main(int argc, char** argv) {

    puts("Welcome to CLisp, version 0.5.0\n");
    puts("Press Ctrl+c to Exit");

#ifdef HAVE_PRELUDE
    printf("\nLoaded libraries\n");
    printf("  * prelude.clisp\n");
#endif
    putchar('\n');


    mpc_parser_t* Comment = mpc_new("comment");
    mpc_parser_t* Boolean = mpc_new("boolean");
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* String = mpc_new("string");
    mpc_parser_t* Symbol = mpc_new("symbol");
    mpc_parser_t* List = mpc_new("list");
    mpc_parser_t* Sexpr = mpc_new("sexpr");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Lisp = mpc_new("lisp");


    mpca_lang(MPCA_LANG_DEFAULT, "                                                    \
        comment  : /;(\\\\.|[^\"])*/;                                                \
        boolean  : /(#t|#f)/;                                                         \
        number   : /-?[0-9]+(\\.[0-9]+)?/;                                            \
        string   : /\"(\\\\.|[^\"])*\"/ ;                                             \
        symbol   : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&\\^\\%\\?]+/ ;                           \
        list     : '[' <expr>* ']' ;                                                  \
        sexpr    : '(' <expr>* ')' ;                                                  \
        expr     : <comment> | <boolean> | <list> | <number> | <symbol> | <string> | <sexpr> ;   \
        lisp     : /^/ <expr>* /$/ ;                                                  \
    ", Comment, Boolean, Number, String, Symbol, List, Sexpr, Expr, Lisp);


    clisp_env_t* env = clisp_env_new();
    clisp_builtin_load_functions(env);

#ifdef HAVE_PRELUDE
    char* lines = strtok(stl_prelude_clisp, "\n");
    while (lines != NULL) {
        REPL(lines, Lisp, env, 1);
        lines = strtok (NULL, "\n");
    }
#endif

    while (1) {
        char* input = readline("> ");
        add_history(input);
        REPL(input, Lisp, env, 0);
        free(input);
    }

    mpc_cleanup(8, Comment, Number, String, Symbol, Sexpr, Expr, Lisp);
    clisp_env_delete(env);

    return 0;
}
