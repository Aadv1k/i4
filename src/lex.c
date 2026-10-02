#include "lex.h"

#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
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
    ['c' - 'a'][0] =
        (Pair){
            .a = "const",
            .b = (void *)(uintptr_t)TOK_CONST,
        },

    ['v' - 'a'][0] = (Pair){
        .a = "var",
        .b = (void *)(uintptr_t)TOK_VAR,
    }};

#define _lexer_advance(l)                                                      \
  (((size_t)((l)->cursor - (l)->buf) < ((l)->buf_len)) ? ((l)->cursor++) : NULL)

#define _lexer_peek(l)                                                         \
  (((size_t)((l)->cursor - (l)->buf) < ((l)->buf_len)) ? ((l)->cursor + 1)     \
                                                       : NULL)

#define _lexer_jump_by(l, offset)                                              \
  ((size_t)(((l)->cursor + (offset)) - (l)->buf) <= (l)->buf_len               \
       ? (l)->cursor += (offset)                                               \
       : NULL)

Lexer *i4_lexer_init(const char *src, size_t len) {
  Lexer *lexer = malloc(sizeof(Lexer));
  if (lexer == NULL)
    return NULL;

  char *buf = strndup(src, len);
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

#define _lexer_match_sign(c) ((c) == '-' || (c) == '+')

int _lexer_try_consume_number(Lexer *lexer) {
  // .67
  // -.6767
  // -12
  // 69.420
  // 1e1 -> 1 * 10^1
  // 1e-3
  // 69_420
  // 69420

  int exponent = 0, decimal = 0, sign = 0;
  const char *ch = lexer->cursor, *orig_ch = lexer->cursor;

  sign = _lexer_match_sign(*ch);

  // Maximally chomp digits until something odd is encountered
  for (;;) {
    ch = lexer->cursor;
    const char *next_ch = _lexer_peek(lexer);

    // TODO: clean this abomination up
    switch (*ch) {
    case 'e':
      if ((exponent || decimal) && (next_ch != NULL || isdigit(*next_ch) || _lexer_match_sign(*next_ch)))
        goto lexer_try_consume_number_error;
      exponent = 1;
      break;
    case '.':
      if (decimal ||
          exponent && (next_ch != NULL ||
                       isdigit(*next_ch) && !(_lexer_match_sign(*next_ch))))
        goto lexer_try_consume_number_error;
      decimal = 1;
      break;
    case '_':
      continue;
    default:
      if (ch == NULL || isspace(*ch)) {
        ptrdiff_t wlen = ch - orig_ch;
        lexer->cur_token = (Token){.type = decimal ? TOK_FLOAT : TOK_INT,
                                   .buf = strndup(orig_ch, wlen),
                                   .buf_len = wlen};
        return 0;
      }

      goto lexer_try_consume_number_error;
      break;
    }

    _lexer_advance(lexer);
  }

  // 67420<-
  // 67420 <-
  // 67.<-
  // 67e<-

lexer_try_consume_number_error:
  return -1;
}

int _lexer_try_consume_word(Lexer *lexer) {
  const char *ch = lexer->cursor, *orig_ch = lexer->cursor;

  // MAXIMAL CHOMP!!!
  while (ch != NULL &&
         (isalnum((unsigned char)*ch) || *ch == '_' || *ch == '$')) {
    ch = lexer->cursor;
    _lexer_advance(lexer);
  }

  ptrdiff_t wlen = ch - orig_ch;

  if (isalnum((unsigned char)*orig_ch) && keywords[*orig_ch - 'a'] != NULL) {
    for (size_t i = 0; i < KWORD_PER_CHAR; ++i) {
      if (keywords[*orig_ch - 'a'][i].a == NULL)
        continue;

      Pair keyword_pair = keywords[*orig_ch - 'a'][i];

      const char *kword = (char *)keyword_pair.a;
      size_t kword_len = strlen(kword);

      if (kword_len <=
          (size_t)(lexer->buf_len - (lexer->buf - lexer->cursor))) {
        if (strncmp(kword, orig_ch, (size_t)wlen) == 0) {
          lexer->cur_token = (Token){
              .type = (TokenType)(uintptr_t)keyword_pair.b
              // TODO: col, line should also be added here
          };
          return 0;
        }
      }
    }
  }

  // try parse it as an identifier in this case, let's see how this bites us in
  // the ass in the futrue
  lexer->cur_token = (Token){
      .type = TOK_IDENT, .buf = strndup(orig_ch, wlen), .buf_len = wlen};

  return 0;
}

int i4_lexer_match_op(Lexer *lexer) {
  switch (*lexer->cursor) {
  case '+':
  case '-':
  case '=':
  case ';':
    lexer->cur_token = (Token){.type = *lexer->cursor};
    break;
  }

  return -1;
}

int i4_lexer_next_token(Lexer *lexer) {
  for (;;) {
    switch (*lexer->cursor) {
    case '+':
    case '-':
    case '=':
    case ';':
      return i4_lexer_match_op(lexer);
      break;
    default: {
      if (isblank(*lexer->cursor))
        continue;

      if (isdigit(*lexer->cursor))
        _lexer_try_consume_number(lexer);

      if (_lexer_try_consume_word(lexer) == 0)
        return 0;

      goto lexer_next_token_error;
    }
    }
  }

lexer_next_token_error:
  return -1;
}

void i4_lexer_free(Lexer *lexer) {
  if (lexer == NULL)
    return;

  free((void *)lexer->buf);
  free(lexer);
}
