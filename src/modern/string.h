#pragma once

#include "integer.h"
#include "memory.h"

typedef struct {
    u8 *base;
    address length;
} String;

typedef struct {
    u8 *base;
    address length;
    address size;
} DynamicString;

#define literal(C_STRING) ((String) {.base = (u8 *)(C_STRING), .length = sizeof((C_STRING)) - 1})

