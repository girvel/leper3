#pragma once

#include "io.c"

typedef struct {
    u8 seconds;
    u8 minutes;
    u8 hours;
    u8 day;
    u8 month;
    u8 year;
} clock_Time;

u8 _clock_read(u8 reg) {
    io_write_byte(0x70, reg);
    u8 bcd = io_read_byte(0x71);
    u8 result = (bcd % 0x10)
        + ((bcd / 0x10) % 0x10) * 10;
    return result;
}

clock_Time clock_read() {
    return (clock_Time) {
        _clock_read(0x00),
        _clock_read(0x02),
        _clock_read(0x04) + 5,
        _clock_read(0x07),
        _clock_read(0x08),
        _clock_read(0x09)
    };
}
