#include "kb.hpp"
#include "kernel/io.hpp"


namespace {
    buffer scancode_map(
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
    );

    buffer shift_scancode_map(
        0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
        '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
        0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '|', '~', 0, '\\',
        'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0
    );

    constexpr u16 data_port = 0x60;
    constexpr u16 status_port = 0x64;

    static bool shift = false;
}

option<u8> kb::read_scancode() {
    if (io::read(status_port) & 0x01) {
        return io::read(data_port);
    }
    return none;
}

option<u8> kb::read() {
    u8 scancode = try(kb::read_scancode());

    if (scancode == kb::Scancode::arrow_prefix) {
        switch (try(kb::read_scancode())) {
            case kb::Scancode::arrow_up: return kb::Key::arrow_up;
            case kb::Scancode::arrow_left: return kb::Key::arrow_left;
            case kb::Scancode::arrow_right: return kb::Key::arrow_right;
            case kb::Scancode::arrow_down: return kb::Key::arrow_down;
            default: return none;
        }
    }

    if (scancode & kb::Scancode::release) {
        scancode ^= kb::Scancode::release;
        switch (scancode) {
            case kb::Scancode::lshift:
            case kb::Scancode::rshift:
                shift = false;
        }
        return none;
    }

    switch (scancode) {
        case kb::Scancode::lshift:
        case kb::Scancode::rshift:
            shift = true;
            return none;
    }

    return (shift ? shift_scancode_map : scancode_map)[scancode];
}

u8 kb::read_blocking() {
    while (true) {
        if (auto in = kb::read().check()) return *in;
    }
}
