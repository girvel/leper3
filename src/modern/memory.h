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
