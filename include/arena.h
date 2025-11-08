/*
 * Copyright (c) 2025, Ibrahim KAIKAA <ibrahimkaikaa@gmail.com>
 * SPDX-License-Identifier: GPL-3.0
 */

#ifndef _ARENA_H
#define _ARENA_H

#include <stdbool.h>
#include <stddef.h>

typedef struct arena_chunk arena_chunk_t;

struct arena_chunk
{
    void *data;
    size_t size;
    size_t capacity;
    arena_chunk_t *next;
};

typedef struct
{
    size_t size;
    arena_chunk_t *first;
    arena_chunk_t *current;
} arena_t;

bool arena_init(arena_t *arena);
void *arena_alloc(arena_t *arena, size_t size);
void arena_destroy(arena_t *arena);

#endif
