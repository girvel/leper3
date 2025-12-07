#include "vga.hpp"

namespace {
    constexpr address video_memory = 0xb8000;
};

vga::Cell *vga::cell(u8 x, u8 y) {
    return (vga::Cell *)video_memory + x * vga::w + y;
}

