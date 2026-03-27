#include "leper3.h"

static u8 clock_read_byte(u8 reg) {
    io_write_byte(0x70, reg);
    u8 bcd = io_read_byte(0x71);
    u8 result = (bcd % 0x10)
        + ((bcd / 0x10) % 0x10) * 10;
    return result;
}

clock_Time clock_read() {
    return (clock_Time) {
        clock_read_byte(0x00),
        clock_read_byte(0x02),
        (clock_read_byte(0x04) + 5) % 24,
        clock_read_byte(0x07),
        clock_read_byte(0x08),
        clock_read_byte(0x09)
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
