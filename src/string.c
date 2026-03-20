#include "modern/integer.h"
#include "modern/memory.h"
#include <stdarg.h>

address str_write_signed(u8 *dest, address cap, i32 integer) {
    address initial_cap = cap;
    if (cap < 2) return 0;

    if (integer < 0) {
        *dest = '-';
        dest++; cap--;
        integer *= -1;
    }

    u8 *before = dest;
    while (true) {
        if (cap == 0) return 0;
        u8 digit = integer % 10;
        integer /= 10;
        *dest = '0' + digit;
        dest++; cap--;
        if (integer == 0) break;
    }

    address len = dest - before;
    for (address i = 0; 2 * i < len; i++) {
        address j = len - 1 - i;
        u8 tmp = before[i];
        before[i] = before[j];
        before[j] = tmp;
    }

    return initial_cap - cap;
}

void str_format_args(u8 *dest, address cap, const u8 *fmt, va_list args);

void str_format(u8 *dest, address cap, const u8 *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    str_format_args(dest, cap, fmt, args);
}

void str_format_args(u8 *dest, address cap, const u8 *fmt, va_list args) {
    for (const u8 *ch = fmt; *ch && cap > 1; ch++) {
        if (*ch != '%') {
            *dest = *ch;
            dest++; cap--;
            continue;
        }

        ch++;

        switch (*ch) {
        case '\0': goto end;
        case 'i': {
            address delta = str_write_signed(dest, cap - 1, va_arg(args, i32));
            if (delta == 0) {
                *dest = '^';
                goto end;
            }
            dest += delta;
            cap -= delta;
        } break;
        default: {
            *dest = '^';
            dest++; cap--;
        } break;
        }
    }

end:
    // assert(cap >= 1);
    *dest = '\0';
    dest++; cap--;
}
