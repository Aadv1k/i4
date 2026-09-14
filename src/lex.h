#ifndef LEX_H
#define LEX_H

#include <stddef.h>

typedef enum {
    // Keywords
    TOK_VAR, TOK_CONST,

    // Literals
    TOK_IDENT, TOK_FLOAT, TOK_INT, TOK_STRING
} TokenType;

typedef struct Token {
    TokenType typ;

    const char* buf;
    size_t buf_len;

    size_t line, col;
} Token;


typedef struct Lexer {
    const char* start;
    const char* cursor;
    size_t len, line;
} Lexer;

Lexer* i4_lexer_init(const char* src, size_t len);

int i4_lex(Lexer* lexer);

void i4_lexer_free(Lexer* lexer);


#endif
