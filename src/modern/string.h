#pragma once

#include "integer.h"
#include "memory.h"
#include "allocator.h"

typedef struct {
    u8 *base;
    address length;
} String;

typedef struct {
    u8 *base;
    address length;
    address size;
} DynamicString;

typedef struct {
    String *base;
    address length;
    address size;
} Strings;

typedef struct {
    String *base;
    address length;
} StringArray;

#define literal(C_STRING) ((String) {.base = (u8 *)(C_STRING), .length = sizeof((C_STRING)) - 1})

// TODO these all can be memory macros
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

StringArray string_split(String target, Allocator *allocator, u8 separator) {
    Strings result = {0};
    DynamicString current_word = {0};

    enumerate (address, i, u8 *, character, &target) {
        bool push = false;
        if (*character == ' ') {
            push = true;
        } else {
            da_append(&current_word, allocator, *character);
        }

        if (current_word.size > 0 && (push || i == target.length - 1)) {
            da_append(&result, allocator, slice(String, current_word));
            current_word = (DynamicString) {0};
        }
    }

    return slice(StringArray, result);
}

u8 _string_digits[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void string_append_signed(DynamicString *target, Allocator *allocator, i32 integer) {
    if (integer < 0) {
        da_append(target, allocator, '-');
    }
    address start = target->size;

    do {
        u8 n = integer % 10;
        integer /= 10;
        da_append(target, allocator, _string_digits[n]);
    } while (integer != 0);

    for (address i = start; i < (start + target->size) / 2; i++) {
        address j = target->size - 1 - i + start;
        u8 tmp = target->base[i];
        target->base[i] = target->base[j];
        target->base[j] = tmp;
    }
}
