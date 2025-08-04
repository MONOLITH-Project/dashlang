#include "lexer_utils.h"
#include "reader.h"
#include <lexer.h>
#include <unity_internals.h>

static test_token_list_t expected_list;
static test_token_list_t actual_list;

void setUp(void)
{
    expected_list = new_token_list();
    actual_list = new_token_list();
}

void tearDown(void)
{
    free_token_list(&actual_list);
    free_token_list(&expected_list);
}

void lex_empty_string(void)
{
    reader_t reader = reader_from_string("");
    lexer_t lexer = lexer_init(&reader);
    init_token_list(
        &expected_list,
        (token_t) {
            .value = "",
            .type = TOKEN_EOF,
            .line = 1,
            .column = 1,
        });
    lex_all(&lexer, &actual_list);
    COMPARE_TOKEN_LISTS(expected_list, actual_list);
}

void lex_keywords(void)
{
    reader_t reader = reader_from_string(
        "break continue fall fn for i16 i8 if let return struct switch u16 u8 union");
    lexer_t lexer = lexer_init(&reader);
    init_token_list(
        &expected_list,
        (token_t) {.value = "break", .type = TOKEN_BREAK, .line = 1, .column = 1},
        (token_t) {.value = "continue", .type = TOKEN_CONTINUE, .line = 1, .column = 7},
        (token_t) {.value = "fall", .type = TOKEN_FALL, .line = 1, .column = 16},
        (token_t) {.value = "fn", .type = TOKEN_FN, .line = 1, .column = 21},
        (token_t) {.value = "for", .type = TOKEN_FOR, .line = 1, .column = 24},
        (token_t) {.value = "i16", .type = TOKEN_I16, .line = 1, .column = 28},
        (token_t) {.value = "i8", .type = TOKEN_I8, .line = 1, .column = 32},
        (token_t) {.value = "if", .type = TOKEN_IF, .line = 1, .column = 35},
        (token_t) {.value = "let", .type = TOKEN_LET, .line = 1, .column = 38},
        (token_t) {.value = "return", .type = TOKEN_RETURN, .line = 1, .column = 42},
        (token_t) {.value = "struct", .type = TOKEN_STRUCT, .line = 1, .column = 49},
        (token_t) {.value = "switch", .type = TOKEN_SWITCH, .line = 1, .column = 56},
        (token_t) {.value = "u16", .type = TOKEN_U16, .line = 1, .column = 63},
        (token_t) {.value = "u8", .type = TOKEN_U8, .line = 1, .column = 67},
        (token_t) {.value = "union", .type = TOKEN_UNION, .line = 1, .column = 70},
        (token_t) {.value = "", .type = TOKEN_EOF, .line = 1, .column = 75});
    lex_all(&lexer, &actual_list);
    COMPARE_TOKEN_LISTS(expected_list, actual_list);
}

void lex_unsigned_integers(void)
{
    reader_t reader = reader_from_string("123 456 789");
    lexer_t lexer = lexer_init(&reader);
    init_token_list(
        &expected_list,
        (token_t) {.value = "123", .type = TOKEN_INTEGER, .line = 1, .column = 1},
        (token_t) {.value = "456", .type = TOKEN_INTEGER, .line = 1, .column = 5},
        (token_t) {.value = "789", .type = TOKEN_INTEGER, .line = 1, .column = 9},
        (token_t) {.value = "", .type = TOKEN_EOF, .line = 1, .column = 12});
    lex_all(&lexer, &actual_list);
    COMPARE_TOKEN_LISTS(expected_list, actual_list);
}

void lex_operators(void)
{
    reader_t reader = reader_from_string("+ - * / = == != < > <= >= ! ; : . , ( ) [ ] { }");
    lexer_t lexer = lexer_init(&reader);
    init_token_list(
        &expected_list,
        (token_t) {.value = "+", .type = TOKEN_PLUS, .line = 1, .column = 1},
        (token_t) {.value = "-", .type = TOKEN_MINUS, .line = 1, .column = 3},
        (token_t) {.value = "*", .type = TOKEN_STAR, .line = 1, .column = 5},
        (token_t) {.value = "/", .type = TOKEN_SLASH, .line = 1, .column = 7},
        (token_t) {.value = "=", .type = TOKEN_EQUAL, .line = 1, .column = 9},
        (token_t) {.value = "==", .type = TOKEN_EQUAL_EQUAL, .line = 1, .column = 11},
        (token_t) {.value = "!=", .type = TOKEN_NOT_EQUAL, .line = 1, .column = 14},
        (token_t) {.value = "<", .type = TOKEN_LESS_THAN, .line = 1, .column = 17},
        (token_t) {.value = ">", .type = TOKEN_GREATER_THAN, .line = 1, .column = 19},
        (token_t) {.value = "<=", .type = TOKEN_LESS_EQUAL, .line = 1, .column = 21},
        (token_t) {.value = ">=", .type = TOKEN_GREATER_EQUAL, .line = 1, .column = 24},
        (token_t) {.value = "!", .type = TOKEN_NOT, .line = 1, .column = 27},
        (token_t) {.value = ";", .type = TOKEN_SEMICOLON, .line = 1, .column = 29},
        (token_t) {.value = ":", .type = TOKEN_COLON, .line = 1, .column = 31},
        (token_t) {.value = ".", .type = TOKEN_DOT, .line = 1, .column = 33},
        (token_t) {.value = ",", .type = TOKEN_COMMA, .line = 1, .column = 35},
        (token_t) {.value = "(", .type = TOKEN_LEFT_PAREN, .line = 1, .column = 37},
        (token_t) {.value = ")", .type = TOKEN_RIGHT_PAREN, .line = 1, .column = 39},
        (token_t) {.value = "[", .type = TOKEN_LEFT_BRACKET, .line = 1, .column = 41},
        (token_t) {.value = "]", .type = TOKEN_RIGHT_BRACKET, .line = 1, .column = 43},
        (token_t) {.value = "{", .type = TOKEN_LEFT_BRACE, .line = 1, .column = 45},
        (token_t) {.value = "}", .type = TOKEN_RIGHT_BRACE, .line = 1, .column = 47},
        (token_t) {.value = "", .type = TOKEN_EOF, .line = 1, .column = 48});
    lex_all(&lexer, &actual_list);
    COMPARE_TOKEN_LISTS(expected_list, actual_list);
}

void lex_separate_with_operators(void)
{
    reader_t reader = reader_from_string("abc=xyz");
    lexer_t lexer = lexer_init(&reader);
    init_token_list(
        &expected_list,
        (token_t) {.value = "abc", .type = TOKEN_IDENTIFIER, .line = 1, .column = 1},
        (token_t) {.value = "=", .type = TOKEN_EQUAL, .line = 1, .column = 4},
        (token_t) {.value = "xyz", .type = TOKEN_IDENTIFIER, .line = 1, .column = 5},
        (token_t) {.value = "", .type = TOKEN_EOF, .line = 1, .column = 8});
    lex_all(&lexer, &actual_list);
    COMPARE_TOKEN_LISTS(expected_list, actual_list);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(lex_empty_string);
    RUN_TEST(lex_keywords);
    RUN_TEST(lex_unsigned_integers);
    RUN_TEST(lex_operators);
    RUN_TEST(lex_separate_with_operators);
    destroy_token_list(&actual_list);
    destroy_token_list(&expected_list);
    return UNITY_END();
}
