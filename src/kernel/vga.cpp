#include "vga.hpp"

namespace {
    constexpr address VideoMemory = 0xb8000;
};

vga::Cell *vga::cell(Vector<u8, 2> position) {
    return (vga::Cell *)VideoMemory + position.x * vga::ScreenSize.width + position.y;
}

