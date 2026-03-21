#pragma once
#include "primitives.h"
#include "varargs.h"

// Write a decimal representation without a null-terminator
address str_write_signed(u8 *dest, address cap, i32 integer);

// Format a string; supports %i
void str_format(u8 *dest, address cap, const u8 *fmt, ...);

// Format a string using va_list
void str_format_args(u8 *dest, address cap, const u8 *fmt, va_list args);
