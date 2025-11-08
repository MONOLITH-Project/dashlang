/*
 * Copyright (c) 2025, Ibrahim KAIKAA <ibrahimkaikaa@gmail.com>
 * SPDX-License-Identifier: GPL-3.0
 */

#include <arena.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <utils.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

#define ARENA_CHUNK_MULTIPLIER 10

static size_t arena_chunk_size = 0;

static size_t get_page_size(void)
{
#ifdef _WIN32
    SYSTEM_INFO si;
    GetSystemInfo(&si);
    return si.dwPageSize;
#else
    long ps = sysconf(_SC_PAGESIZE);
    return (size_t) ps;
#endif
}

static void *page_alloc(size_t size)
{
#ifdef _WIN32
    return VirtualAlloc(NULL, size, MEM_COMMIT, PAGE_READWRITE);
#else
    void *ptr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    return ptr == MAP_FAILED ? NULL : ptr;
#endif
}

static void page_free(void *ptr, size_t size)
{
#ifdef _WIN32
    VirtualFree(ptr, 0, MEM_RELEASE);
#else
    munmap(ptr, size);
#endif
}

static arena_chunk_t *_arena_new_chunk(size_t size)
{
    arena_chunk_t *chunk = malloc(sizeof(arena_chunk_t));
    if (chunk == NULL)
        return NULL;

    chunk->size = 0;
    chunk->capacity = size;
    chunk->next = NULL;
    chunk->data = page_alloc(size);
    if (chunk->data == NULL) {
        free(chunk);
        return NULL;
    }

    return chunk;
}

bool arena_init(arena_t *arena)
{
    if (arena_chunk_size == 0) {
        arena_chunk_size = ARENA_CHUNK_MULTIPLIER * get_page_size();
    }
    arena_chunk_t *head = _arena_new_chunk(arena_chunk_size);
    if (head == NULL)
        return false;
    arena->current = head;
    arena->first = head;
    arena->size = arena_chunk_size;

    return true;
}

void *arena_alloc(arena_t *arena, size_t size)
{
    if (size + arena->current->size >= arena->current->capacity) {
        size_t chunk_capacity = size > arena_chunk_size ? ALIGN_UP(size, arena_chunk_size)
                                                        : arena_chunk_size;
        arena_chunk_t *chunk = _arena_new_chunk(chunk_capacity);
        if (chunk == NULL)
            return NULL;
        arena->current->next = chunk;
        arena->current = chunk;
        arena->size += chunk_capacity;
    }

    void *ptr = arena->current->data + arena->current->size;
    arena->current->size += size;

    return ptr;
}

void arena_destroy(arena_t *arena)
{
    arena_chunk_t *chunk = arena->first;
    while (chunk != NULL) {
        arena_chunk_t *next = chunk->next;
        page_free(chunk->data, chunk->capacity);
        free(chunk);
        chunk = next;
    }
}
