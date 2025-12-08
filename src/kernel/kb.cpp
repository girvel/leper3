#include "kb.hpp"
#include "kernel/io.hpp"


namespace {
    buffer scancode_map(
        0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
        '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
        0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
        'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
    );

    constexpr u16 data_port = 0x60;
    constexpr u16 status_port = 0x64;
}

option<u8> kb::read_scancode() {
    if (io::read(status_port) & 0x01) {
        return io::read(data_port);
    }
    return none;
}

option<u8> kb::read() {
    u8 scancode = try(kb::read_scancode());
    return scancode_map[scancode];
}
