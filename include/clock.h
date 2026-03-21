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

// Read the current clock state
clock_Time clock_read();

// Get the current clock frame
u64 clock_frame();

// Get an approximation of the current millisecond
u32 clock_ms();

// Conversion rate between frames & seconds
extern u32 clock_MHz;

