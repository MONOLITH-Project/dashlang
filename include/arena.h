/*
 * Copyright (c) 2025, Ibrahim KAIKAA <ibrahimkaikaa@gmail.com>
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef _ARENA_H
#define _ARENA_H

#include <stddef.h>

typedef struct
{
    size_t size;
    size_t capacity;
    void *data;
} arena_t;

arena_t arena_init(size_t capacity);
void *arena_alloc(arena_t *arena, size_t size);
void arena_destroy(arena_t *arena);

#endif
