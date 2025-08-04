/*
 * Copyright (c) 2025, Ibrahim KAIKAA <ibrahimkaikaa@gmail.com>
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef _LEXER_H
#define _LEXER_H

#include <bits/stdint-uintn.h>
#include <stddef.h>

typedef struct
{
    char *source;
    size_t line;
    size_t column;
    size_t position;
} lexer_t;

typedef enum {
    TOKEN_INVALID = -1,
    TOKEN_EOF = 0,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_FALL,
    TOKEN_FN,
    TOKEN_FOR,
    TOKEN_I16,
    TOKEN_I8,
    TOKEN_IF,
    TOKEN_LET,
    TOKEN_RETURN,
    TOKEN_STRUCT,
    TOKEN_SWITCH,
    TOKEN_U16,
    TOKEN_U8,
    TOKEN_UNION,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_STRING,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS_THAN,
    TOKEN_GREATER_THAN,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_NOT,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_DOT,
    TOKEN_COMMA,
    TOKEN_LEFT_PAREN,
    TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACKET,
    TOKEN_RIGHT_BRACKET,
    TOKEN_LEFT_BRACE,
    TOKEN_RIGHT_BRACE
} token_type_t;

typedef struct
{
    char value[16];
    size_t line;
    size_t column;
    token_type_t type;
} token_t;

lexer_t lexer_init(char *source);
token_t lexer_next(lexer_t *lexer);
token_t lexer_peek(lexer_t *lexer);
token_t lexer_prev(lexer_t *lexer);

#endif
