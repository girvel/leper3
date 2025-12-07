#include "vga.hpp"

namespace {
    constexpr address VideoMemory = 0xb8000;
};

vga::Cell *vga::cell(vector<u8, 2> position) {
    return reinterpret_cast<vga::Cell *>(VideoMemory)
        + position.x + position.y * vga::ScreenSize.height;
}

void vga::clear(vga::ColorPair color) {
    vga::Cell *video_memory = vga::cell({0, 0});

    for (address i = 0; i < vga::ScreenSize.width * vga::ScreenSize.height; i++) {
        video_memory[i].character = ' ';
        video_memory[i].color = color;
    }
}

void vga::write(u8x2 position, slice<u8> text, ColorPair color) {
    u8x2 current_position = position;
    for (u8 character : text) {
        if (character == '\n') {
            current_position.x = position.x;
            current_position.y++;
        } else {
            vga::Cell *cell = vga::cell(current_position);
            cell->character = character;
            cell->color = color;
            current_position.x++;
        }
    }
}
