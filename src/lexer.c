/*
 * Copyright (c) 2025, Ibrahim KAIKAA <ibrahimkaikaa@gmail.com>
 * SPDX-License-Identifier: GPL-3.0
 */

#include <assert.h>
#include <ctype.h>
#include <lexer.h>
#include <stdbool.h>
#include <string.h>

lexer_t lexer_init(reader_t *reader)
{
    lexer_t lexer = {.reader = reader, .line = 1, .column = 1};
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
    else if (!strcmp(token, "struct"))
        return TOKEN_STRUCT;
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
           || c == '>' || c == '.' || c == '!' || c == ';' || c == ':' || c == ',' || c == '('
           || c == ')' || c == '{' || c == '}' || c == '[' || c == ']';
}

static bool _is_separator(char c)
{
    return c == ' ' || c == '\t' || c == '\n' || c == '\0' || _is_operator(c);
}

static bool _is_identifier_start(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

#define SINGLECHAR_CASE(c, t) \
    case c: \
        token.type = t; \
        token.value[0] = c; \
        token.value[1] = '\0'; \
        return token

#define TWOCHAR_CASE(fchar, schar, fcase, scase) \
    case fchar: \
        current = reader_peek(lexer->reader); \
        if (current == schar) { \
            reader_next(lexer->reader); \
            lexer->column++; \
            token.type = fcase; \
            token.value[0] = fchar; \
            token.value[1] = schar; \
            token.value[2] = '\0'; \
        } else { \
            token.type = scase; \
            token.value[0] = fchar; \
            token.value[1] = '\0'; \
        } \
        break

token_t lexer_next(lexer_t *lexer)
{
    token_t token = {
        .type = TOKEN_EOF,
        .value = "",
        .line = lexer->line,
        .column = lexer->column,
    };

    char current;

    /* Skip whitespace */
    while (1) {
        current = reader_next(lexer->reader);
        if (current == '\n') {
            lexer->line++;
            lexer->column = 1;
        } else if (current == ' ' || current == '\t') {
            lexer->column++;
        } else if (current == '\0') {
            return token;
        } else {
            break;
        }
    }

    token.line = lexer->line;
    token.column = lexer->column;
    lexer->column++;

    if (_is_identifier_start(current)) {
        unsigned int i = 0;
        token.value[i++] = current;

        char next;
        while (1) {
            next = reader_peek(lexer->reader);
            if (_is_separator(next)) {
                break;
            }
            current = reader_next(lexer->reader);
            lexer->column++;
            if (i < sizeof(token.value) - 1) {
                token.value[i++] = current;
            } else {
                break;
            }
        }
        token.value[i] = '\0';
        token.type = _classify_token(token.value);
        return token;
    } else if (isdigit(current)) {
        unsigned int i = 0;
        token.value[i++] = current;

        char next;
        while (isdigit((next = reader_peek(lexer->reader)))) {
            current = reader_next(lexer->reader);
            lexer->column++;
            token.value[i++] = current;
        }
        token.value[i] = '\0';
        token.type = TOKEN_INTEGER;
        return token;
    }

    switch (current) {
        SINGLECHAR_CASE('+', TOKEN_PLUS);
        SINGLECHAR_CASE('-', TOKEN_MINUS);
        SINGLECHAR_CASE('*', TOKEN_STAR);
        SINGLECHAR_CASE('/', TOKEN_SLASH);
        SINGLECHAR_CASE(';', TOKEN_SEMICOLON);
        SINGLECHAR_CASE(':', TOKEN_COLON);
        SINGLECHAR_CASE('.', TOKEN_DOT);
        SINGLECHAR_CASE(',', TOKEN_COMMA);
        SINGLECHAR_CASE('(', TOKEN_LEFT_PAREN);
        SINGLECHAR_CASE(')', TOKEN_RIGHT_PAREN);
        SINGLECHAR_CASE('[', TOKEN_LEFT_BRACKET);
        SINGLECHAR_CASE(']', TOKEN_RIGHT_BRACKET);
        SINGLECHAR_CASE('{', TOKEN_LEFT_BRACE);
        SINGLECHAR_CASE('}', TOKEN_RIGHT_BRACE);
        TWOCHAR_CASE('=', '=', TOKEN_EQUAL_EQUAL, TOKEN_EQUAL);
        TWOCHAR_CASE('!', '=', TOKEN_NOT_EQUAL, TOKEN_NOT);
        TWOCHAR_CASE('>', '=', TOKEN_GREATER_EQUAL, TOKEN_GREATER_THAN);
        TWOCHAR_CASE('<', '=', TOKEN_LESS_EQUAL, TOKEN_LESS_THAN);
    default:
        token.type = TOKEN_INVALID;
        token.value[0] = current;
        token.value[1] = '\0';
        break;
    }

    return token;
}
