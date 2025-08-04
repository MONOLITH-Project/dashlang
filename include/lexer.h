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

typedef enum : uint8_t {
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
} token_type_t;

typedef struct
{
    char value[16];
    token_type_t type;
    size_t line;
    size_t column;
} token_t;

lexer_t lexer_init(char *source);
token_t lexer_next(lexer_t *lexer);
token_t lexer_peek(lexer_t *lexer);
token_t lexer_prev(lexer_t *lexer);

#endif
