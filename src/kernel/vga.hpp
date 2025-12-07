#include <modern.hpp>

namespace vga {
    constexpr u8 h = 25;
    constexpr u8 w = 80;

    struct __attribute__((packed)) Cell {
        u8 character;
        u8 color;
    };

    Cell *cell(u8 x, u8 y);
}

