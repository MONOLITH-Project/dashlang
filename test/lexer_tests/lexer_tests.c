#include "lexer_utils.h"
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
    lexer_t lexer = lexer_init("");
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
    lexer_t lexer = lexer_init("break continue fall fn for if let return switch union");
    init_token_list(
        &expected_list,
        (token_t) {.value = "break", .type = TOKEN_BREAK, .line = 1, .column = 1},
        (token_t) {.value = "continue", .type = TOKEN_CONTINUE, .line = 1, .column = 7},
        (token_t) {.value = "fall", .type = TOKEN_FALL, .line = 1, .column = 16},
        (token_t) {.value = "fn", .type = TOKEN_FN, .line = 1, .column = 21},
        (token_t) {.value = "for", .type = TOKEN_FOR, .line = 1, .column = 24},
        (token_t) {.value = "if", .type = TOKEN_IF, .line = 1, .column = 25},
        (token_t) {.value = "let", .type = TOKEN_LET, .line = 1, .column = 28},
        (token_t) {.value = "return", .type = TOKEN_RETURN, .line = 1, .column = 31},
        (token_t) {.value = "switch", .type = TOKEN_SWITCH, .line = 1, .column = 38},
        (token_t) {.value = "union", .type = TOKEN_UNION, .line = 1, .column = 45},
        (token_t) {.value = "", .type = TOKEN_EOF, .line = 1, .column = 52});
    lex_all(&lexer, &actual_list);
    COMPARE_TOKEN_LISTS(expected_list, actual_list);
}

void lex_unsigned_integers(void)
{
    lexer_t lexer = lexer_init("123 456 789");
    init_token_list(
        &expected_list,
        (token_t) {.value = "123", .type = TOKEN_INTEGER, .line = 1, .column = 1},
        (token_t) {.value = "456", .type = TOKEN_INTEGER, .line = 1, .column = 5},
        (token_t) {.value = "789", .type = TOKEN_INTEGER, .line = 1, .column = 9},
        (token_t) {.value = "", .type = TOKEN_EOF, .line = 1, .column = 13});
    lex_all(&lexer, &actual_list);
    COMPARE_TOKEN_LISTS(expected_list, actual_list);
}

int main(void)
{
    UNITY_BEGIN();
    RUN_TEST(lex_empty_string);
    RUN_TEST(lex_keywords);
    RUN_TEST(lex_unsigned_integers);
    destroy_token_list(&actual_list);
    destroy_token_list(&expected_list);
    return UNITY_END();
}
