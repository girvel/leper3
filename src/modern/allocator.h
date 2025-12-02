#pragma once

#include "memory.h"

typedef struct {
    void *payload;
    void *(*allocate_raw)(void *payload, void *prev, address length);
    void (*free_raw)(void *payload, void *base);
} Allocator;

// TODO non-1-byte lengths
#define allocate(TYPE, ALLOCATOR, PREV, LENGTH) ({ \
    __typeof__ (ALLOCATOR) __allocator = (ALLOCATOR); \
    void *__prev = (PREV).base; \
    address __length = (LENGTH); \
    TYPE __result = {.length = __length}; \
    __result.base = __allocator->allocate_raw( \
        __allocator->payload, __prev, __length * sizeof(__result.base[0]) \
    ); \
    __result; \
})
