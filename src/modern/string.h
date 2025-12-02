#pragma once

#include "integer.h"
#include "memory.h"
#include "allocator.h"
#include "time.h"
#include <stdarg.h>

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
            append(&current_word, allocator, *character);
        }

        if (current_word.size > 0 && (push || i == target.length - 1)) {
            append(&result, allocator, to_fat(String, current_word));
            current_word = (DynamicString) {0};
        }
    }

    return to_fat(StringArray, result);
}

u8 _string_digits[10] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

void string_write_signed(DynamicString *target, Allocator *allocator, i32 integer) {
    if (integer < 0) {
        append(target, allocator, '-');
        integer *= -1;
    }
    address start = target->size;

    do {
        u8 n = integer % 10;
        integer /= 10;
        append(target, allocator, _string_digits[n]);
    } while (integer != 0);

    for (address i = start; i < (start + target->size) / 2; i++) {
        address j = target->size - 1 - i + start;
        u8 tmp = target->base[i];
        target->base[i] = target->base[j];
        target->base[j] = tmp;
    }
}

bool string_to_signed(String str, i32 *result) {
    i32 sign = 1;
    if (str.base[0] == '-') {
        sign = -1;
        str.base++;
        str.length--;
    }

    i32 value = 0;
    foreach(u8 *, character, &str) {
        if (*character < '0' || *character > '9') return false;
        value = value * 10 + (*character - '0');
    }

    *result = sign * value;
    return true;
}

void string_format(DynamicString *target, Allocator *allocator, String format, ...) {
    va_list args;
    va_start(args, format);

    foreach (u8 *, character, &format) {
        if (*character == '%') {
            character++;

            if (*character == '%') {
                append(target, allocator, '%');
            } else if (*character == 'i') {
                string_write_signed(target, allocator, va_arg(args, i32));
            } else if (*character == 't') {
                Time *time = va_arg(args, Time*);
                string_format(
                    target, allocator, literal("20%02i-%02i-%02i %02i:%02i:%02i"),
                    time->year, time->month, time->day, time->hours, time->minutes, time->seconds
                );
            } else if (*character == '0') {
                character++;
                u32 padding = 0;
                do {
                    if (*character < '0' || *character > '9') {
                        append(target, allocator, '^');
                        continue;
                    }

                    padding *= 10;
                    padding += *character - '0';
                    character++;
                } while (*character != 'i');

                address size_before = target->size;
                string_write_signed(target, allocator, va_arg(args, i32));
                padding -= target->size - size_before;

                extend(target, allocator, padding);
                target->size += padding;
                for (address i = size_before; i < size_before + padding; i++) {
                    target->base[i + padding] = target->base[i];
                    target->base[i] = '0';
                }
            } else {
                append(target, allocator, '^');
            }

            continue;
        }

        append(target, allocator, *character);
    }
}
