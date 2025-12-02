#pragma once

#include "integer.h"
#include "macros.h"

typedef u32 address;

typedef struct {
    void *base;
    address length;
} Fat;

#define null ((Fat) {0})
#define is_null(X) ((X).base == 0)

#define static_region(TYPE, NAME, LENGTH) \
    TYPE NAME = {0}; \
    __typeof__ (NAME.base[0]) concat3(__, NAME, _base)[LENGTH]; \
    NAME.base = concat3(__, NAME, _base); \
    NAME.length = LENGTH;

#define foreach(TYPE, VARNAME, TARGET) \
    for (TYPE VARNAME = (TARGET)->base; VARNAME < (TARGET)->base + (TARGET)->length; VARNAME++)

#define slice(SLICE_TYPE, X) ({ \
    __typeof__ (X) __x = (X); \
    (SLICE_TYPE) {.base = __x.base, .length = __x.size}; \
})

#define fat_cast(TYPE, REGION) ({ \
    __typeof__ (REGION) __region = (REGION); \
    (TYPE) { \
        .base = __region.base, \
        .length = __region.length \
    }; \
})

#define copy(SOURCE, DESTINATION) do { \
    __typeof__ (SOURCE) __source = (SOURCE); \
    __typeof__ (DESTINATION) __destination = (DESTINATION); \
    for (address i = 0; i < MIN2(__source.length, __destination.length); i++) { \
        __destination.base[i] = __source.base[i]; \
    } \
} while (0)

