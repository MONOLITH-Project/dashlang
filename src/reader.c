/*
 * Copyright (c) 2025, Ibrahim KAIKAA <ibrahimkaikaa@gmail.com>
 * SPDX-License-Identifier: GPL-3.0
 */

#include <reader.h>

static char _string_reader_next(reader_t *reader)
{
    const char *string = reader->internal;
    if (!string)
        return 0;
    char c = string[reader->position++];
    return c;
}

static char _string_reader_peek(reader_t *reader)
{
    const char *string = reader->internal;
    if (!string)
        return 0;
    char c = string[reader->position];
    return c;
}

reader_t reader_from_string(const char *string)
{
    return (reader_t) {
        .internal = (void *) string,
        .position = 0,
        .next = _string_reader_next,
        .peek = _string_reader_peek,
    };
}

char reader_peek(reader_t *reader) {
    return reader->peek(reader);
}

char reader_next(reader_t *reader) {
    return reader->next(reader);
}
