#pragma once

#include "modern/time.h"
#include "io.c"

u8 _clock_read(u8 reg) {
    io_write_byte(0x70, reg);
    u8 bcd = io_read_byte(0x71);
    u8 result = (bcd % 0x10)
        + ((bcd / 0x10) % 0x10) * 10;
    return result;
}

Time clock_read() {
    return (Time) {
        _clock_read(0x00),
        _clock_read(0x02),
        (_clock_read(0x04) + 5) % 24,
        _clock_read(0x07),
        _clock_read(0x08),
        _clock_read(0x09)
    };
}

u64 clock_frame() {
    u32 low, high;
    __asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
    return ((u64)high << 32) | low;
}

u32 clock_MHz = 4 * 1024 * 1024;

u32 clock_ms() {
    return (u32)clock_frame() / clock_MHz;
}
