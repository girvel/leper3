#pragma once

#include "memory.h"

typedef struct {
    void *payload;
    void *(*allocate_raw)(void *payload, void *prev, address length);
    void (*free_raw)(void *payload, void *base);
} Allocator;

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

#define free(ALLOCATOR, FAT) do { \
    __typeof__ (ALLOCATOR) __allocator = (ALLOCATOR); \
    __allocator->free_raw(__allocator->payload, (FAT).base); \
} while (0)

// TODO stop passing the allocator by reference
#define extend(FAT, ALLOCATOR, LENGTH) do { \
    __typeof__ (FAT) extend__fat = (FAT); \
    if (LENGTH > extend__fat->length) { \
        __typeof__ (*extend__fat) __new_array = allocate( \
            __typeof__ (*extend__fat), (ALLOCATOR), *extend__fat, (LENGTH) \
        ); \
        if (__new_array.base != extend__fat->base) { \
            copy(*extend__fat, __new_array); \
            extend__fat->base = __new_array.base; \
        } \
        extend__fat->length = __new_array.length; \
    } \
} while (0)

#define append(LIST, ALLOCATOR, ELEMENT) do { \
    __typeof__ (LIST) append__list = (LIST); \
    __typeof__ (ALLOCATOR) append__allocator = (ALLOCATOR); \
    __typeof__ (ELEMENT) append__element = (ELEMENT); \
    if (append__list->size >= append__list->length) { \
        extend(append__list, append__allocator, append__list->length == 0 ? 16 : append__list->length * 2); \
    } \
    append__list->base[append__list->size++] = append__element; \
} while (0)

#define append_many(LIST, ALLOCATOR, ELEMENTS) do { \
    __typeof__ (LIST) append__list = (LIST); \
    __typeof__ (ALLOCATOR) append__allocator = (ALLOCATOR); \
    __typeof__ (ELEMENTS) append__elements = (ELEMENTS); \
    if (append__list->size + append__elements.length > append__list->length) { \
        address new_length = append__list->length == 0 ? 16 : append__list->length * 2; \
        while (append__list->size + append__elements.length > new_length) new_length *= 2; \
        extend(append__list, append__allocator, new_length); \
    } \
    __typeof__ (append__elements) append__destination = { \
        .base = append__list->base + append__list->length, \
        .length = append__elements.length \
    }; \
    copy(append__elements, append__destination); \
} while (0)
