/*
 * Copyright (c) 2025, Ibrahim KAIKAA <ibrahimkaikaa@gmail.com>
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef _LEXER_H
#define _LEXER_H

#include <bits/stdint-uintn.h>
#include <reader.h>
#include <stddef.h>

typedef struct
{
    reader_t *reader;
    size_t line;
    size_t column;
} lexer_t;

typedef enum {
    TOKEN_INVALID = -1,
    TOKEN_EOF = 0,
    TOKEN_BREAK,
    TOKEN_CONTINUE,
    TOKEN_FALL,
    TOKEN_FOR,
    TOKEN_FUNCTION,
    TOKEN_I16,
    TOKEN_I32,
    TOKEN_I64,
    TOKEN_I8,
    TOKEN_IF,
    TOKEN_IMPL,
    TOKEN_INTERFACE,
    TOKEN_LET,
    TOKEN_RETURN,
    TOKEN_CLASS,
    TOKEN_SWITCH,
    TOKEN_TYPE,
    TOKEN_ENUM,
    TOKEN_BOOL,
    TOKEN_DEFAULT,
    TOKEN_ELSE,
    TOKEN_INT,
    TOKEN_SKIP,
    TOKEN_STRING,
    TOKEN_UINT,
    TOKEN_NULL,
    TOKEN_U16,
    TOKEN_U32,
    TOKEN_U64,
    TOKEN_U8,
    TOKEN_F32,
    TOKEN_F64,
    TOKEN_IDENTIFIER,
    TOKEN_INTEGER,
    TOKEN_STRING_LITERAL,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_STAR,
    TOKEN_SLASH,
    TOKEN_PERCENT,
    TOKEN_EQUAL,
    TOKEN_EQUAL_EQUAL,
    TOKEN_NOT_EQUAL,
    TOKEN_LESS_THAN,
    TOKEN_GREATER_THAN,
    TOKEN_LESS_EQUAL,
    TOKEN_GREATER_EQUAL,
    TOKEN_PLUS_EQUAL,
    TOKEN_MINUS_EQUAL,
    TOKEN_STAR_EQUAL,
    TOKEN_SLASH_EQUAL,
    TOKEN_PERCENT_EQUAL,
    TOKEN_NOT,
    TOKEN_ARROW,
    TOKEN_AND,
    TOKEN_OR,
    TOKEN_SEMICOLON,
    TOKEN_COLON,
    TOKEN_DOUBLE_COLON,
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

lexer_t lexer_init(reader_t *reader);
token_t lexer_next(lexer_t *lexer);
token_t lexer_peek(lexer_t *lexer);
token_t lexer_prev(lexer_t *lexer);

#endif
