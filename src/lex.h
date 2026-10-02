#ifndef LEX_H
#define LEX_H

#include <stdint.h>
#include <stddef.h>

// Allows us to represent both enums and single-chars as token types
typedef uint8_t TokenType;

typedef enum {
    // Keywords
    TOK_VAR, TOK_CONST,

    // Literals
    TOK_IDENT, TOK_FLOAT, TOK_INT, TOK_STRING,

    TOK_EOF
} TokenLabel;

typedef struct Token {
    TokenType type;
    const char* buf;
    size_t buf_len;
} Token;


typedef struct Lexer {
    const char* buf;
    size_t buf_len;

    const char* cursor;

    Token cur_token;

    size_t line;
} Lexer;

Lexer* i4_lexer_init(const char* src, size_t len);

int i4_lexer_next_token(Lexer* lexer);

void i4_lexer_free(Lexer* lexer);


#endif
