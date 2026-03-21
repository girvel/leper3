#pragma once
#include "primitives.h"

typedef struct {
    u8 seconds;
    u8 minutes;
    u8 hours;
    u8 day;
    u8 month;
    u8 year;
} clock_Time;

clock_Time clock_read();
u64 clock_frame();
u32 clock_ms();
extern u32 clock_MHz;

