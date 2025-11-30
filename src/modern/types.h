#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef u32 address;

typedef struct {
    void *base;
    address length;
} Fat;

#define null ((Fat) {0})
#define is_null(X) ((X).base == 0)

