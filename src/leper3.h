#pragma once
#include "gcc/stdbool.h"  // IWYU pragma: keep
#include "gcc/stdarg.h"  // IWYU pragma: keep

// -------------------------------------------------------------------------------------------------
// [SECTION] Primitive types
// -------------------------------------------------------------------------------------------------

typedef __UINT8_TYPE__  u8;
typedef __UINT16_TYPE__ u16;
typedef __UINT32_TYPE__ u32;
typedef __UINT64_TYPE__ u64;

typedef __INT8_TYPE__  i8;
typedef __INT16_TYPE__ i16;
typedef __INT32_TYPE__ i32;
typedef __INT64_TYPE__ i64;

typedef u32 usize;
typedef i64 isize;

#define LEN(X) ((usize) (sizeof(X) / sizeof(*X)))

// -------------------------------------------------------------------------------------------------
// [SECTION] Input/Output
// -------------------------------------------------------------------------------------------------

// Read a byte from the given port
u8 io_read_byte(u16 port);

// Write a byte to the given port
void io_write_byte(u16 port, u8 data);

// -------------------------------------------------------------------------------------------------
// [SECTION] Clock
// -------------------------------------------------------------------------------------------------

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

// -------------------------------------------------------------------------------------------------
// [SECTION] Keyboard
// -------------------------------------------------------------------------------------------------

// Non-ASCII characters returned by kb_read()
typedef enum {
    kb_Key_up = 128,
    kb_Key_down = 129,
    kb_Key_left = 130,
    kb_Key_right = 131,
} kb_Key;

// Reads current keyboard state; returns 0 if nothing's pressed, else a valid character or a kb_Key;
// any kb_Key is >= 128.
u8 kb_read();

// -------------------------------------------------------------------------------------------------
// [SECTION] Power
// -------------------------------------------------------------------------------------------------

// Trigger an immedate reboot
void power_reboot();

// -------------------------------------------------------------------------------------------------
// [SECTION] Memory pages
// -------------------------------------------------------------------------------------------------

// Set all pages as unused; do not call this
void page_reset();

void *page_allocate();

void page_free(void *page);

#define PAGE_BASE 0x100000
#define PAGE_SIZE 4096
#define PAGE_PAGES 256

// -------------------------------------------------------------------------------------------------
// [SECTION] Random
// -------------------------------------------------------------------------------------------------

/// Returns the next pseudo-random number; the algo is very very simple
i32 random_next();

// -------------------------------------------------------------------------------------------------
// [SECTION] String
// -------------------------------------------------------------------------------------------------

// Write a decimal representation without a null-terminator
usize str_write_signed(u8 *dest, usize cap, i32 integer);

// Format a string; supports %i
void str_format(u8 *dest, usize cap, const u8 *fmt, ...);

// Format a string using va_list
void str_format_args(u8 *dest, usize cap, const u8 *fmt, va_list args);

