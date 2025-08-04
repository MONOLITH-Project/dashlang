/*
 * Copyright (c) 2025, Ibrahim KAIKAA <ibrahimkaikaa@gmail.com>
 * SPDX-License-Identifier: GPL-3.0
 */

#include <assert.h>
#include <lexer.h>
#include <stdbool.h>
#include <string.h>

lexer_t lexer_init(char *source)
{
    lexer_t lexer = {.source = source, .position = 0, .line = 1, .column = 1};
    return lexer;
}

static token_type_t _classify_token(char *token)
{
    if (!strcmp(token, "break"))
        return TOKEN_BREAK;
    else if (!strcmp(token, "continue"))
        return TOKEN_CONTINUE;
    else if (!strcmp(token, "fall"))
        return TOKEN_FALL;
    else if (!strcmp(token, "fn"))
        return TOKEN_FN;
    else if (!strcmp(token, "for"))
        return TOKEN_FOR;
    else if (!strcmp(token, "i16"))
        return TOKEN_I16;
    else if (!strcmp(token, "i8"))
        return TOKEN_I8;
    else if (!strcmp(token, "if"))
        return TOKEN_IF;
    else if (!strcmp(token, "let"))
        return TOKEN_LET;
    else if (!strcmp(token, "return"))
        return TOKEN_RETURN;
    else if (!strcmp(token, "switch"))
        return TOKEN_SWITCH;
    else if (!strcmp(token, "u16"))
        return TOKEN_U16;
    else if (!strcmp(token, "u8"))
        return TOKEN_U8;
    else if (!strcmp(token, "union"))
        return TOKEN_UNION;

    return TOKEN_IDENTIFIER;
}

static bool _is_operator(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '!' || c == '<'
           || c == '>';
}

static bool _is_separator(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\0' || _is_operator(c);
}

static bool _is_whitespace(char c)
{
    return c == ' ' || c == '\t';
}

static bool _is_identifier_start(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

static bool _is_digit(char c)
{
    return c >= '0' && c <= '9';
}

#define TOKEN(ttoken, ttype) \
    (token_t) \
    { \
        .type = ttype, .value = ttoken, .line = lexer->line, .column = lexer->column, \
    }

token_t lexer_next(lexer_t *lexer)
{
    token_t token = {
        .type = TOKEN_EOF,
        .value = "",
        .line = lexer->line,
        .column = lexer->column,
    };

    if (lexer->source[lexer->position] == '\0')
        return token;

skip_whitespace:
    /* Skip whitespace */
    while (_is_whitespace(lexer->source[lexer->position])) {
        lexer->position++;
        lexer->column++;
    }

    /* Handle newlines and EOF */
    if (lexer->source[lexer->position] == '\n') {
        lexer->line++;
        lexer->column = 1;
        lexer->position++;
        goto skip_whitespace;
    } else if (lexer->source[lexer->position] == '\0') {
        return token;
    }

    if (_is_identifier_start(lexer->source[lexer->position])) {
        unsigned int i = 0;
        for (i = 0; !_is_separator(lexer->source[lexer->position]); i++) {
            assert(i < sizeof(token.value));
            token.value[i] = lexer->source[lexer->position];
            lexer->position++;
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = _classify_token(token.value);
        token.line = lexer->line;
        token.column = lexer->column - i;
        return token;
    } else if (_is_digit(lexer->source[lexer->position])) {
        unsigned int i = 0;
        for (i = 0; _is_digit(lexer->source[lexer->position]); i++) {
            assert(i < sizeof(token.value));
            token.value[i] = lexer->source[lexer->position];
            lexer->position++;
            lexer->column++;
        }
        token.value[i] = '\0';
        token.type = TOKEN_INTEGER;
        token.line = lexer->line;
        token.column = lexer->column - i;
        return token;
    }

    switch (lexer->source[lexer->position]) {
    case '+':
        token = TOKEN("+", TOKEN_PLUS);
        lexer->position++;
        break;
    case '-':
        token = TOKEN("-", TOKEN_MINUS);
        lexer->position++;
        break;
    case '*':
        token = TOKEN("*", TOKEN_STAR);
        lexer->position++;
        break;
    case '/':
        token = TOKEN("/", TOKEN_SLASH);
        lexer->position++;
        break;
    case '=':
        if (lexer->source[lexer->position + 1] == '=') {
            token = TOKEN("==", TOKEN_EQUAL_EQUAL);
            lexer->position += 2;
        } else {
            token = TOKEN("=", TOKEN_EQUAL);
            lexer->position++;
        }
        break;
    case '!':
        if (lexer->source[lexer->position + 1] == '=') {
            token = TOKEN("!=", TOKEN_NOT_EQUAL);
            lexer->position += 2;
        } else {
            token = TOKEN("!", TOKEN_NOT);
            lexer->position++;
        }
        break;
    case '>':
        if (lexer->source[lexer->position + 1] == '=') {
            token = TOKEN(">=", TOKEN_GREATER_EQUAL);
            lexer->position += 2;
        } else {
            token = TOKEN(">", TOKEN_GREATER_THAN);
            lexer->position++;
        }
        break;
    case '<':
        if (lexer->source[lexer->position + 1] == '=') {
            token = TOKEN("<=", TOKEN_LESS_EQUAL);
            lexer->position += 2;
        } else {
            token = TOKEN("<", TOKEN_LESS_THAN);
            lexer->position++;
        }
        break;
    }

    return token;
}
