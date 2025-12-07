#include "vga.hpp"

namespace {
    constexpr address VideoMemory = 0xb8000;
};

vga::Cell *vga::cell(Vector<u8, 2> position) {
    return (vga::Cell *)VideoMemory + position.items[0] * vga::w + position.items[1];
}

