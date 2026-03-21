#pragma once
#include "primitives.h"
#include "varargs.h"

address str_write_signed(u8 *dest, address cap, i32 integer);
void str_format(u8 *dest, address cap, const u8 *fmt, ...);
void str_format_args(u8 *dest, address cap, const u8 *fmt, va_list args);
