#include <lexer.h>
#include <reader.h>
#include <unity_internals.h>

#include "./lexer_utils.h"

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
        "break continue fall function for i16 i32 i64 i8 if impl interface let return class switch "
        "type bool default else int skip string uint null u16 u32 u64 u8 f32 f64 enum");
    lexer_t lexer = lexer_init(&reader);
    init_token_list(
        &expected_list,
        (token_t) {.value = "break", .type = TOKEN_BREAK, .line = 1, .column = 1},
        (token_t) {.value = "continue", .type = TOKEN_CONTINUE, .line = 1, .column = 7},
        (token_t) {.value = "fall", .type = TOKEN_FALL, .line = 1, .column = 16},
        (token_t) {.value = "function", .type = TOKEN_FUNCTION, .line = 1, .column = 21},
        (token_t) {.value = "for", .type = TOKEN_FOR, .line = 1, .column = 30},
        (token_t) {.value = "i16", .type = TOKEN_I16, .line = 1, .column = 34},
        (token_t) {.value = "i32", .type = TOKEN_I32, .line = 1, .column = 38},
        (token_t) {.value = "i64", .type = TOKEN_I64, .line = 1, .column = 42},
        (token_t) {.value = "i8", .type = TOKEN_I8, .line = 1, .column = 46},
        (token_t) {.value = "if", .type = TOKEN_IF, .line = 1, .column = 49},
        (token_t) {.value = "impl", .type = TOKEN_IMPL, .line = 1, .column = 52},
        (token_t) {.value = "interface", .type = TOKEN_INTERFACE, .line = 1, .column = 57},
        (token_t) {.value = "let", .type = TOKEN_LET, .line = 1, .column = 67},
        (token_t) {.value = "return", .type = TOKEN_RETURN, .line = 1, .column = 71},
        (token_t) {.value = "class", .type = TOKEN_CLASS, .line = 1, .column = 78},
        (token_t) {.value = "switch", .type = TOKEN_SWITCH, .line = 1, .column = 84},
        (token_t) {.value = "type", .type = TOKEN_TYPE, .line = 1, .column = 91},
        (token_t) {.value = "bool", .type = TOKEN_BOOL, .line = 1, .column = 96},
        (token_t) {.value = "default", .type = TOKEN_DEFAULT, .line = 1, .column = 101},
        (token_t) {.value = "else", .type = TOKEN_ELSE, .line = 1, .column = 109},
        (token_t) {.value = "int", .type = TOKEN_INT, .line = 1, .column = 114},
        (token_t) {.value = "skip", .type = TOKEN_SKIP, .line = 1, .column = 118},
        (token_t) {.value = "string", .type = TOKEN_STRING, .line = 1, .column = 123},
        (token_t) {.value = "uint", .type = TOKEN_UINT, .line = 1, .column = 130},
        (token_t) {.value = "null", .type = TOKEN_NULL, .line = 1, .column = 135},
        (token_t) {.value = "u16", .type = TOKEN_U16, .line = 1, .column = 140},
        (token_t) {.value = "u32", .type = TOKEN_U32, .line = 1, .column = 144},
        (token_t) {.value = "u64", .type = TOKEN_U64, .line = 1, .column = 148},
        (token_t) {.value = "u8", .type = TOKEN_U8, .line = 1, .column = 152},
        (token_t) {.value = "f32", .type = TOKEN_F32, .line = 1, .column = 155},
        (token_t) {.value = "f64", .type = TOKEN_F64, .line = 1, .column = 159},
        (token_t) {.value = "enum", .type = TOKEN_ENUM, .line = 1, .column = 163},
        (token_t) {.value = "", .type = TOKEN_EOF, .line = 1, .column = 167});
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
    reader_t reader = reader_from_string(
        "+ - * / % = == != < > <= >= ! ; : :: . , ( ) [ ] { } -> && || += -= *= /= %=");
    lexer_t lexer = lexer_init(&reader);
    init_token_list(
        &expected_list,
        (token_t) {.value = "+", .type = TOKEN_PLUS, .line = 1, .column = 1},
        (token_t) {.value = "-", .type = TOKEN_MINUS, .line = 1, .column = 3},
        (token_t) {.value = "*", .type = TOKEN_STAR, .line = 1, .column = 5},
        (token_t) {.value = "/", .type = TOKEN_SLASH, .line = 1, .column = 7},
        (token_t) {.value = "%", .type = TOKEN_PERCENT, .line = 1, .column = 9},
        (token_t) {.value = "=", .type = TOKEN_EQUAL, .line = 1, .column = 11},
        (token_t) {.value = "==", .type = TOKEN_EQUAL_EQUAL, .line = 1, .column = 13},
        (token_t) {.value = "!=", .type = TOKEN_NOT_EQUAL, .line = 1, .column = 16},
        (token_t) {.value = "<", .type = TOKEN_LESS_THAN, .line = 1, .column = 19},
        (token_t) {.value = ">", .type = TOKEN_GREATER_THAN, .line = 1, .column = 21},
        (token_t) {.value = "<=", .type = TOKEN_LESS_EQUAL, .line = 1, .column = 23},
        (token_t) {.value = ">=", .type = TOKEN_GREATER_EQUAL, .line = 1, .column = 26},
        (token_t) {.value = "!", .type = TOKEN_NOT, .line = 1, .column = 29},
        (token_t) {.value = ";", .type = TOKEN_SEMICOLON, .line = 1, .column = 31},
        (token_t) {.value = ":", .type = TOKEN_COLON, .line = 1, .column = 33},
        (token_t) {.value = "::", .type = TOKEN_DOUBLE_COLON, .line = 1, .column = 35},
        (token_t) {.value = ".", .type = TOKEN_DOT, .line = 1, .column = 38},
        (token_t) {.value = ",", .type = TOKEN_COMMA, .line = 1, .column = 40},
        (token_t) {.value = "(", .type = TOKEN_LEFT_PAREN, .line = 1, .column = 42},
        (token_t) {.value = ")", .type = TOKEN_RIGHT_PAREN, .line = 1, .column = 44},
        (token_t) {.value = "[", .type = TOKEN_LEFT_BRACKET, .line = 1, .column = 46},
        (token_t) {.value = "]", .type = TOKEN_RIGHT_BRACKET, .line = 1, .column = 48},
        (token_t) {.value = "{", .type = TOKEN_LEFT_BRACE, .line = 1, .column = 50},
        (token_t) {.value = "}", .type = TOKEN_RIGHT_BRACE, .line = 1, .column = 52},
        (token_t) {.value = "->", .type = TOKEN_ARROW, .line = 1, .column = 54},
        (token_t) {.value = "&&", .type = TOKEN_AND, .line = 1, .column = 57},
        (token_t) {.value = "||", .type = TOKEN_OR, .line = 1, .column = 60},
        (token_t) {.value = "+=", .type = TOKEN_PLUS_EQUAL, .line = 1, .column = 63},
        (token_t) {.value = "-=", .type = TOKEN_MINUS_EQUAL, .line = 1, .column = 66},
        (token_t) {.value = "*=", .type = TOKEN_STAR_EQUAL, .line = 1, .column = 69},
        (token_t) {.value = "/=", .type = TOKEN_SLASH_EQUAL, .line = 1, .column = 72},
        (token_t) {.value = "%=", .type = TOKEN_PERCENT_EQUAL, .line = 1, .column = 75},
        (token_t) {.value = "", .type = TOKEN_EOF, .line = 1, .column = 77});
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

void lex_ignore_comments(void)
{
    reader_t reader = reader_from_string("// This is a comment\nx");
    lexer_t lexer = lexer_init(&reader);
    init_token_list(
        &expected_list,
        (token_t) {.value = "x", .type = TOKEN_IDENTIFIER, .line = 2, .column = 1},
        (token_t) {.value = "", .type = TOKEN_EOF, .line = 2, .column = 2});
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
    RUN_TEST(lex_ignore_comments);
    destroy_token_list(&actual_list);
    destroy_token_list(&expected_list);
    return UNITY_END();
}
