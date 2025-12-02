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

#define da_append(ARRAY, ALLOCATOR, ELEMENT) do { \
    __typeof__ (ARRAY) da_append__array = (ARRAY); \
    __typeof__ (ALLOCATOR) da_append__allocator = (ALLOCATOR); \
    __typeof__ (ELEMENT) da_append__element = (ELEMENT); \
    if (da_append__array->size >= da_append__array->length) { \
        __typeof__ (*da_append__array) __new_array = allocate( \
            __typeof__ (*da_append__array), da_append__allocator, *da_append__array, \
            da_append__array->length == 0 ? 16 : da_append__array->length * 2 \
        ); \
        copy(*da_append__array, __new_array); \
        da_append__array->base = __new_array.base; \
        da_append__array->length = __new_array.length; \
    } \
    da_append__array->base[da_append__array->size++] = da_append__element; \
} while (0)

