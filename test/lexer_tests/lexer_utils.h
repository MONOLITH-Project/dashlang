#include <lexer.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unity.h>

typedef struct
{
    token_t *list;
    size_t size;
    size_t capacity;
} test_token_list_t;

test_token_list_t new_token_list(void)
{
    test_token_list_t list;
    list.list = malloc(sizeof(token_t));
    list.size = 0;
    list.capacity = 1;
    return list;
}

void token_list_append(test_token_list_t *list, token_t token)
{
    if (list->size == list->capacity) {
        list->capacity *= 2;
        list->list = realloc(list->list, sizeof(token_t) * list->capacity);
    }
    list->list[list->size++] = token;
}

void init_token_list(test_token_list_t *list, token_t first_token, ...)
{
    token_list_append(list, first_token);
    if (first_token.type == TOKEN_EOF)
        return;

    va_list args;
    va_start(args, first_token);

    token_t token;
    do {
        token = va_arg(args, token_t);
        token_list_append(list, token);
    } while (token.type != TOKEN_EOF);

    va_end(args);
}

void free_token_list(test_token_list_t *list)
{
    list->size = 0;
}

void destroy_token_list(test_token_list_t *list)
{
    free(list->list);
    memset(list, 0, sizeof(test_token_list_t));
}

void lex_all(lexer_t *lexer, test_token_list_t *list)
{
    token_t token;
    do {
        token = lexer_next(lexer);
        token_list_append(list, token);
    } while (token.type != TOKEN_EOF);
}

#define COMPARE_TOKEN_LISTS(expected, actual) \
    do { \
        TEST_ASSERT_EQUAL((expected).size, (actual).size); \
        size_t i; \
        for (i = 0; i < (expected).size; i++) { \
            TEST_ASSERT_EQUAL((expected).list[i].type, (actual).list[i].type); \
            TEST_ASSERT_EQUAL_STRING((expected).list[i].value, (actual).list[i].value); \
        } \
    } while (0)
