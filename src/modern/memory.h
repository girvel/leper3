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

#define static_region(NAME, LENGTH) \
    u8 concat3(__, NAME, _base)[LENGTH]; \
    Fat NAME = {.base = concat3(__, NAME, _base), .length = LENGTH};

#define foreach(TYPE, VARNAME, TARGET) \
    for (TYPE VARNAME = (TARGET)->base; VARNAME < (TARGET)->base + (TARGET)->length; VARNAME++)
