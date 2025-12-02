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
#define extend_exact(FAT, ALLOCATOR, LENGTH) do { \
    __typeof__ (FAT) extend_exact__fat = (FAT); \
    if (LENGTH > extend_exact__fat->length) { \
        __typeof__ (*extend_exact__fat) __new_array = allocate( \
            __typeof__ (*extend_exact__fat), (ALLOCATOR), *extend_exact__fat, (LENGTH) \
        ); \
        if (__new_array.base != extend_exact__fat->base) { \
            copy(*extend_exact__fat, __new_array); \
            extend_exact__fat->base = __new_array.base; \
        } \
        extend_exact__fat->length = __new_array.length; \
    } \
} while (0)

#define extend(FATTER, ALLOCATOR, LENGTH) do { \
    __typeof__ (FATTER) extend__fatter = (FATTER); \
    __typeof__ (LENGTH) extend__length = (LENGTH); \
    address new_length = extend__fatter->length == 0 ? 16 : extend__fatter->length * 2; \
    while (extend__fatter->size + extend__length > new_length) new_length *= 2; \
    extend_exact(extend__fatter, (ALLOCATOR), new_length); \
} while (0)

#define append(LIST, ALLOCATOR, ELEMENT) do { \
    __typeof__ (LIST) append__list = (LIST); \
    __typeof__ (ALLOCATOR) append__allocator = (ALLOCATOR); \
    __typeof__ (ELEMENT) append__element = (ELEMENT); \
    if (append__list->size >= append__list->length) { \
        extend_exact(append__list, append__allocator, append__list->length == 0 ? 16 : append__list->length * 2); \
    } \
    append__list->base[append__list->size++] = append__element; \
} while (0)

#define append_many(LIST, ALLOCATOR, ELEMENTS) do { \
    __typeof__ (LIST) append__list = (LIST); \
    __typeof__ (ALLOCATOR) append__allocator = (ALLOCATOR); \
    __typeof__ (ELEMENTS) append__elements = (ELEMENTS); \
    if (append__list->size + append__elements.length > append__list->length) { \
        extend(append__list, append__allocator, append__elements.length); \
    } \
    __typeof__ (append__elements) append__destination = { \
        .base = append__list->base + append__list->length, \
        .length = append__elements.length \
    }; \
    copy(append__elements, append__destination); \
} while (0)
