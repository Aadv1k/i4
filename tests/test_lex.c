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

void test_lex_invalid_kword(void) {
    const char* src = "constantinople";

    Lexer* l = i4_lexer_init(src, strlen(src));

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(
        l->cur_token.type == TOK_IDENT
    );
    munit_assert_string_equal(l->cur_token.buf, src);

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

void test_lex_ident_underscores(void) {
    const char* src = "__";

    Lexer* l = i4_lexer_init(src, strlen(src));

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(l->cur_token.type == TOK_IDENT);
    munit_assert_string_equal(l->cur_token.buf, src);
    munit_assert(l->cur_token.buf_len == strlen(src));

    i4_lexer_free(l);
}

void test_lex_ident_dollar_embedded_keywords(void) {
    const char* src = "$var$const";

    Lexer* l = i4_lexer_init(src, strlen(src));

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(l->cur_token.type == TOK_IDENT);
    munit_assert_string_equal(l->cur_token.buf, src);
    munit_assert(l->cur_token.buf_len == strlen(src));

    i4_lexer_free(l);
}

void test_lex_ident_keyword_prefix(void) {
    const char* src = "varconst";

    Lexer* l = i4_lexer_init(src, strlen(src));

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(l->cur_token.type == TOK_IDENT);
    munit_assert_string_equal(l->cur_token.buf, src);
    munit_assert(l->cur_token.buf_len == strlen(src));

    i4_lexer_free(l);
}

void test_lex_ident_single_dollar(void) {
    const char* src = "$";

    Lexer* l = i4_lexer_init(src, strlen(src));

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(l->cur_token.type == TOK_IDENT);
    munit_assert_string_equal(l->cur_token.buf, src);
    munit_assert(l->cur_token.buf_len == strlen(src));

    i4_lexer_free(l);
}

void test_lex_ident_underscore_digits(void) {
    const char* src = "_123";

    Lexer* l = i4_lexer_init(src, strlen(src));

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(l->cur_token.type == TOK_IDENT);
    munit_assert_string_equal(l->cur_token.buf, src);
    munit_assert(l->cur_token.buf_len == strlen(src));

    i4_lexer_free(l);
}

void test_lex_int(void) {
    const char* src = "69420";

    Lexer* l = i4_lexer_init(src, strlen(src));

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(l->cur_token.type == TOK_INT);
    munit_assert_string_equal(l->cur_token.buf, src);
    munit_assert(l->cur_token.buf_len == strlen(src));

    i4_lexer_free(l);
}

void test_lex_float(void) {
    const char* src = "69.420";

    Lexer* l = i4_lexer_init(src, strlen(src));

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(l->cur_token.type == TOK_FLOAT);
    munit_assert_string_equal(l->cur_token.buf, src);
    munit_assert(l->cur_token.buf_len == strlen(src));

    i4_lexer_free(l);
}

void test_lex_number_separator(void) {
    const char* src = "69_420";

    Lexer* l = i4_lexer_init(src, strlen(src));

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(l->cur_token.type == TOK_INT);
    munit_assert_string_equal(l->cur_token.buf, src);
    munit_assert(l->cur_token.buf_len == strlen(src));

    i4_lexer_free(l);
}

void test_lex_number_scientific(void) {
    const char* src = "1e3";

    Lexer* l = i4_lexer_init(src, strlen(src));

    munit_assert(i4_lexer_next_token(l) == 0);
    munit_assert(l->cur_token.type == TOK_INT || l->cur_token.type == TOK_FLOAT);
    munit_assert_string_equal(l->cur_token.buf, src);
    munit_assert(l->cur_token.buf_len == strlen(src));

    i4_lexer_free(l);
}

void test_lex(void) {
    /* Identifier and keywords */

//     test_lex_basic_keyword();
//     test_lex_invalid_kword();
//     test_lex_const_var();
//     test_lex_ident_underscores();
//     test_lex_ident_dollar_embedded_keywords();
//     test_lex_ident_keyword_prefix();
//     test_lex_ident_single_dollar();
//     test_lex_ident_underscore_digits();
//
    /* El Numeros */

    test_lex_int();
    test_lex_float();
    test_lex_number_separator();
    test_lex_number_scientific();
}
