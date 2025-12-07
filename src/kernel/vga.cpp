#include "vga.hpp"

namespace {
    constexpr address VideoMemory = 0xb8000;
};

vga::Cell *vga::cell(u8 x, u8 y) {
    return (vga::Cell *)VideoMemory + x * vga::w + y;
}

