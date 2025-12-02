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

bool string_equal(String a, String b) {
    if (a.length != b.length) return false;
    for (address i = 0; i < a.length; i++) {
        if (a.base[i] != b.base[i]) return false;
    }
    return true;
}

String string_sub(String a, address start, address end) {
    if (end <= start) {
        return (String) {0};
    }

    if (start > a.length) start = a.length;
    if (end > a.length) end = a.length;

    return (String) {
        .base = a.base + start,
        .length = end - start,
    };
}

bool string_starts_with(String target, String prefix) {
    return string_equal(string_sub(target, 0, prefix.length), prefix);
}
