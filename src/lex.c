#include "lex.h"

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


/*
x = 35;
var x = 35;
var y = 34;

const z = x + y;
*/


typedef struct Pair {
    void *a, *b;
} Pair;

#define KWORD_POSSIBLE_CHARS ('z' - 'a' + 1)
#define KWORD_PER_CHAR 4

static Pair keywords[KWORD_POSSIBLE_CHARS][KWORD_PER_CHAR] = {
    ['c' - 'a'][0] = (Pair){
        .a = "const",
        .b = (void*)(uintptr_t)TOK_CONST,
    },

    ['v' - 'a'][0] = (Pair){
        .a = "var",
        .b = (void*)(uintptr_t)TOK_VAR,
    }
};


#define _lexer_advance(l) \
    (((size_t)((l)->cursor - (l)->buf) < ((l)->buf_len)) \
        ? ((l)->cursor++) \
        : NULL)

#define _lexer_peek(l) \
    (((size_t)((l)->cursor - (l)->buf) < ((l)->buf_len)) \
        ? ((l)->cursor + 1) \
        : NULL)

#define _lexer_jump_by(l, offset) \
    ((size_t)(((l)->cursor + (offset)) - (l)->buf) <= (l)->buf_len \
        ? (l)->cursor += (offset) \
        : NULL)

Lexer* i4_lexer_init(const char* src, size_t len) {
    Lexer* lexer = malloc(sizeof(Lexer));
    if (lexer == NULL) return NULL;

    char* buf = strndup(src, len);
    if (buf == NULL) {
        free(lexer);
        return NULL;
    }

    *lexer = (Lexer){
        .buf = buf,
        .buf_len = len,
        .cursor = buf,
        .line = 1,
    };

    return lexer;
}

int _lexer_try_consume_keyword(Lexer* lexer) {
    unsigned char ch = *lexer->cursor;

    if (keywords[ch - 'a'] == NULL) return -1;

    for (size_t i = 0; i < KWORD_PER_CHAR; ++i) {
        if (keywords[ch - 'a'][i].a == NULL) continue;

        Pair keyword_pair = keywords[ch - 'a'][i];

        const char* word = (char*)keyword_pair.a;
        size_t word_len = strlen(word);

        if (word_len > (size_t)(lexer->buf_len - (lexer->buf - lexer->cursor))) continue;

        int matched = 1;
        for (size_t j = 0; j < word_len; ++j) {
            if (word[j] != *(lexer->cursor+j)) {
                matched = 0;
                break;
            };
        }

        assert(
            _lexer_jump_by(lexer, word_len) != NULL
        );

        const char* next = _lexer_peek(lexer);
        if (next != NULL && isspace(*lexer->cursor)) _lexer_advance(lexer);
        ;

        if (matched) {
            lexer->cur_token = (Token){
                .type = (TokenType)(uintptr_t)keyword_pair.b
                // TODO: col, line should also be added here
            };
            return 0;
        }
    }

    return -1;
}

int i4_lexer_next_token(Lexer* lexer) {
    switch (*lexer->cursor) {
        default: {
             if (_lexer_try_consume_keyword(lexer) == 0) return 0;
             goto lexer_next_token_error;
        }
    }

lexer_next_token_error:
    return -1;
}

void i4_lexer_free(Lexer* lexer) {
    if (lexer == NULL) return;

    free((void*)lexer->buf);
    free(lexer);
}
