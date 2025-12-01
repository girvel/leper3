#include "modern/integer.h"

static inline u8 io_read_byte(u16 port) {
    u8 result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

