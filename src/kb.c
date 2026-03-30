#include "leper3.h"

#define KB_DATA_PORT 0x60
#define KB_STATUS_PORT 0x64

static const char kb_scancode_map[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

static const char kb_shift_scancode_map[] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '|', '~', 0, '\\',
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0
};

typedef enum {
    Scancode_lshift = 0x2A,
    Scancode_rshift = 0x46,
    Scancode_arrow_magic = 0xE0,
    Scancode_release = 0x80,
} Scancode;

static u8 kb_read_scancode() {
    if (io_read_byte(KB_STATUS_PORT) & 0x01) {
        return io_read_byte(KB_DATA_PORT);
    }
    return 0;
}

static bool _kb_shift = false;

kb_Reading kb_read() {
    kb_Reading result = {0};
    u8 scancode = kb_read_scancode();

    if (scancode == Scancode_arrow_magic) {
        switch (kb_read_scancode()) {
            case 0x48: result.key = kb_up; break;
            case 0x4B: result.key = kb_left; break;
            case 0x4D: result.key = kb_right; break;
            case 0x50: result.key = kb_down; break;
        }
        return result;
    }

    if (scancode & Scancode_release) {
        scancode = scancode ^ Scancode_release;
        switch (scancode) {
            case Scancode_lshift:
            case Scancode_rshift:
                _kb_shift = false;
        }

        return result;
    }

    switch (scancode) {
        case Scancode_lshift:
        case Scancode_rshift:
            _kb_shift = true;
            result.key = kb_shift;
            return result;
    }

    if (_kb_shift) {
        result.character = scancode < LEN(kb_shift_scancode_map)
            ? kb_shift_scancode_map[scancode] : 0;
    } else {
        result.character = scancode < LEN(kb_scancode_map)
            ? kb_scancode_map[scancode] : 0;
    }
    result.key = result.character == 0 ? 0 : scancode;
    return result;
}

