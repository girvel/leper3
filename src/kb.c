#pragma once

#include "modern/integer.h"
#include "io.c"


#define KB_DATA_PORT 0x60
#define KB_STATUS_PORT 0x64

u8 kb_scancode_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

#define KB_SCANCODE_RELEASE 0x80

u8 kb_read_scancode() {
    if (io_read_byte(KB_STATUS_PORT) & 0x01) {
        return io_read_byte(KB_DATA_PORT);
    }
    return 0;
}

u8 kb_read() {
    u8 scancode = kb_read_scancode();
    if (scancode == 0 || scancode & KB_SCANCODE_RELEASE) return 0;
    return kb_scancode_map[scancode];
}

