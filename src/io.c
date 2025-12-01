#include "modern/integer.h"

static inline u8 io_read_byte(u16 port) {
    u8 result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

static inline void io_write_byte(u16 port, u8 data) {
    __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
}

