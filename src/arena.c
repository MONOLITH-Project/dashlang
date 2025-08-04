/*
 * Copyright (c) 2025, Ibrahim KAIKAA <ibrahimkaikaa@gmail.com>
 * SPDX-License-Identifier: GPL-3.0
 */

#include <arena.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

arena_t arena_init(size_t capacity)
{
    arena_t arena = {.data = malloc(capacity), .capacity = capacity, .size = 0};
    assert(arena.data != NULL);
    return arena;
}

void *arena_alloc(arena_t *arena, size_t size)
{
    assert(arena != NULL);
    assert(size > 0);

    if (arena->size + size > arena->capacity) {
        return NULL;
    }

    void *ptr = arena->data + arena->size;
    arena->size += size;
    return ptr;
}

void arena_destroy(arena_t *arena)
{
    free(arena->data);
    memset(arena, 0, sizeof(arena_t));
}
