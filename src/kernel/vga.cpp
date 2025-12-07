#include "vga.hpp"

namespace {
    constexpr address VideoMemory = 0xb8000;
};

vga::Cell *vga::cell(Vector<u8, 2> position) {
    return reinterpret_cast<vga::Cell *>(VideoMemory)
        + position.x * vga::ScreenSize.width + position.y;
}

void vga::clear(vga::ColorPair color) {
    vga::Cell *video_memory = vga::cell({0, 0});

    for (address i = 0; i < vga::ScreenSize.width * vga::ScreenSize.height; i++) {
        video_memory[i].character = ' ';
        video_memory[i].color = color;
    }
}
