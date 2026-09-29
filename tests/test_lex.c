#include <munit/munit.h>
#include <string.h>
#include "../src/lex.h"

void test_lex_basic_keyword(void) {
    const char* src = "const";

    Lexer* l = i4_lexer_init(src, strlen(src));

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(
        l->cur_token.type == TOK_CONST
    );

    i4_lexer_free(l);
}

void test_lex_const_var(void) {
    const char* src = "const var";

    Lexer* l = i4_lexer_init(src, strlen(src));

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(
        l->cur_token.type == TOK_CONST
    );

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(
        l->cur_token.type == TOK_VAR
    );

    i4_lexer_free(l);
}

void test_lex(void) {
    test_lex_basic_keyword();
    test_lex_const_var();
}
