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
            copy(*append__array, __new_array); \
            append__array->base = __new_array.base; \
        } \
        append__array->length = __new_array.length; \
    } \
} while (0)

#define append(LIST, ALLOCATOR, ELEMENT) do { \
    __typeof__ (LIST) append__array = (LIST); \
    __typeof__ (ALLOCATOR) append__allocator = (ALLOCATOR); \
    __typeof__ (ELEMENT) append__element = (ELEMENT); \
    if (append__array->size >= append__array->length) { \
        extend(append__array, append__allocator, append__array->length == 0 ? 16 : append__array->length * 2); \
    } \
    append__array->base[append__array->size++] = append__element; \
} while (0)

// #define da_extend(LIST, ALLOCATOR, ELEMENTS) do { \
//     __typeof__ (LIST) da_extend__list = (LIST); \
//     __typeof__ (ALLOCATOR) append__allocator = (ALLOCATOR); \
//     __typeof__ (ELEMENTS) da_extend__elements = (ELEMENTS); \
//     if (da_extend__list->size + da_extend__elements->length > da_extend__list->length) { \
//         address new_length = MAX2(16, da_extend__list->length * 2); \
//         while (da_extend__list->size + da_extend__elements->length > new_length) new_length *= 2; \
//         __typeof__ (*da_extend__list) __new_list = allocate( \
//             __typeof__ (*da_extend__list), da_extend__allocator, *da_extend_list, new_length \
//         ); \
//         copy(*da_extend__list, __new_list); \
//         da_extend__list->base = __new_array
// } while(0)
