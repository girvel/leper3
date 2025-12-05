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

#define fat(TYPE) struct { TYPE *base; address length; }

#define def_region(TYPE, NAME, ...) \
    __typeof__ (*((TYPE) {}).base) concat3(_, NAME, _base)[] = __VA_ARGS__; \
    TYPE NAME = { \
        .base = concat3(_, NAME, _base), \
        .length = sizeof(concat3(_, NAME, _base)) / sizeof(*concat3(_, NAME, _base)) \
    };

#define at(FAT, INDEX) ({ \
    __typeof__ (FAT) __fat = (FAT); \
    __typeof__ (INDEX) __index = (INDEX); \
    __index >= 0 && __index < __fat.length ? __fat.base + __index : 0; \
})

#define static_region(TYPE, NAME, LENGTH) \
    TYPE NAME = {0}; \
    __typeof__ (NAME.base[0]) concat3(__, NAME, _base)[LENGTH]; \
    NAME.base = concat3(__, NAME, _base); \
    NAME.length = LENGTH;

#define enumerate(TYPE_I, I, TYPE, VARNAME, TARGET) \
    TYPE_I I = 0; \
    __typeof__ (TARGET) __target = (TARGET); \
    for (TYPE VARNAME = __target->base; I < __target->length; VARNAME = __target->base + ++I)

#define foreach(TYPE, VARNAME, TARGET) \
    for (TYPE VARNAME = (TARGET)->base; VARNAME < (TARGET)->base + (TARGET)->length; VARNAME++)

#define to_fat(SLICE_TYPE, X) ({ \
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

void copy_raw(void *source, void *destination, address length) {
    u8 *_source = source;
    u8 *_destination = destination;
    for (address i = 0; i < length; i++) {
        _destination[i] = _source[i];
    }
}

#define copy(SOURCE, DESTINATION) do { \
    __typeof__ (SOURCE) __source = (SOURCE); \
    __typeof__ (DESTINATION) __destination = (DESTINATION); \
    for (address i = 0; i < MIN2(__source.length, __destination.length); i++) { \
        __destination.base[i] = __source.base[i]; \
    } \
} while (0)

