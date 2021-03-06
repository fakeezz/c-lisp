#include <types.h>
#include "ptest.h"

#include "builtin.h"
#include "../helper.h"

PT_SUITE(suite_builtin_exception) {

    /**
     * Test error
     */
    PT_TEST(test_exception_error) {
        clisp_env_t* env = clisp_env_new();
        clisp_expr_t* expr = clisp_expr_create(clisp_chunk_str("Some error"));
        clisp_chunk_t* result = clisp_builtin_exception_error(expr, env);

        PT_ASSERT(result->type == CLISP_ERROR);
        PT_ASSERT_STR_EQ(result->value.string, "Some error");

        clisp_chunk_free(result);
        clisp_env_free(env);
    }

    PT_TEST(test_exception_error_throws_with_invalid_args_count) {
        clisp_env_t* env = clisp_env_new();
        clisp_chunk_t* result = clisp_builtin_exception_error(clisp_expr_new(), env);

        PT_ASSERT(result->type == CLISP_ERROR);
        PT_ASSERT_STR_EQ(result->value.string, "Incorrect number of arguments. Got: 0, Expected: 1");

        clisp_chunk_free(result);
        clisp_env_free(env);
    }

    PT_TEST(test_exception_error_throws_with_invalid_args_type) {
        clisp_env_t* env = clisp_env_new();
        clisp_expr_t* expr = clisp_expr_create(clisp_chunk_number(1));
        clisp_chunk_t* result = clisp_builtin_exception_error(expr, env);

        PT_ASSERT(result->type == CLISP_ERROR);
        PT_ASSERT_STR_EQ(result->value.string, "Incorrect type of argument. Got: Number, Expected: String");

        clisp_chunk_free(result);
        clisp_env_free(env);
    }

    /**
     * Test error check type
     */
    PT_TEST(test_exception_is_error) {
        clisp_env_t* env = clisp_env_new();
        clisp_expr_t* expr = clisp_expr_create(clisp_chunk_error("Some error"));
        clisp_chunk_t* result = clisp_builtin_exception_is_error(expr, env);

        PT_ASSERT(result->type == CLISP_BOOL);
        PT_ASSERT(result->value.boolean == true);

        clisp_chunk_free(result);
        clisp_env_free(env);
    }

    PT_TEST(test_exception_is_error_throws_arg_count) {
        clisp_env_t* env = clisp_env_new();
        clisp_expr_t* expr = clisp_expr_new();
        clisp_chunk_t* result = clisp_builtin_exception_is_error(expr, env);

        PT_ASSERT(result->type == CLISP_ERROR);

        clisp_chunk_free(result);
        clisp_env_free(env);
    }

}