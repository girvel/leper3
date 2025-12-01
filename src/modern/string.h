#pragma once

#include "integer.h"
#include "memory.h"

typedef struct {
    u8 *base;
    address length;
} String;

#define LITERAL(C_STRING) ((String) {.base = (u8 *)(C_STRING), .length = sizeof((C_STRING)) - 1})

