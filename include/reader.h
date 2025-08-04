/*
 * Copyright (c) 2025, Ibrahim KAIKAA <ibrahimkaikaa@gmail.com>
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef _READER_H
#define _READER_H

#include <stddef.h>

typedef struct reader reader_t;

struct reader
{
    void *internal;
    size_t position;
    char (*next)(reader_t *reader);
    char (*peek)(reader_t *reader);
};

reader_t reader_from_string(const char *string);
char reader_peek(reader_t *reader);
char reader_next(reader_t *reader);

#endif
