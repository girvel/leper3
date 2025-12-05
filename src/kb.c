#pragma once

#include "modern/integer.h"
#include "modern/string.h"
#include "io.c"
#include "modern/memory.h"


#define KB_DATA_PORT 0x60
#define KB_STATUS_PORT 0x64

def_region(String, kb_scancode_map, {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
});

def_region(String, kb_shift_scancode_map, {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '|', '~', 0, '\\',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0
});

typedef enum {
    kb_Key_up = 128,
    kb_Key_down = 129,
    kb_Key_left = 130,
    kb_Key_right = 131,
} kb_Key;


typedef enum {
    kb_Scancode_lshift = 0x2A,
    kb_Scancode_rshift = 0x46,
    kb_Scancode_release = 0x80,
} kb_Scancode;

u8 kb_read_scancode() {
    if (io_read_byte(KB_STATUS_PORT) & 0x01) {
        return io_read_byte(KB_DATA_PORT);
    }
    return 0;
}

static bool _kb_shift = false;

u8 kb_read() {
    u8 scancode = kb_read_scancode();

    if (scancode == 0xE0) {
        switch (kb_read_scancode()) {
            case 0x48: return kb_Key_down;
            case 0x4B: return kb_Key_left;
            case 0x4D: return kb_Key_right;
            case 0x50: return kb_Key_down;
            default: return 0;
        }
    }

    if (scancode & kb_Scancode_release) {
        scancode = scancode ^ kb_Scancode_release;
        switch (scancode) {
            case kb_Scancode_lshift:
            case kb_Scancode_rshift:
                _kb_shift = false;
        }

        return 0;
    }

    switch (scancode) {
        case kb_Scancode_lshift:
        case kb_Scancode_rshift:
            _kb_shift = true;
            return 0;
    }

    u8 *key = at(_kb_shift ? kb_shift_scancode_map : kb_scancode_map, scancode);
    return key ? *key : 0;
}

