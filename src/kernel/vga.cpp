#include "vga.hpp"
#include "io.hpp"

namespace {
    constexpr address VideoMemory = 0xb8000;
};

option<vga::Cell *> vga::cell(vector<u8, 2> position) {
    if (position.x >= vga::ScreenSize.width || position.y >= vga::ScreenSize.height) return none;
    return reinterpret_cast<vga::Cell *>(VideoMemory)
        + position.x + position.y * vga::ScreenSize.width;
}

void vga::clear(vga::ColorPair color) {
    vga::Cell *video_memory = vga::cell({0, 0}).unwrap();

    for (address i = 0; i < vga::ScreenSize.width * vga::ScreenSize.height; i++) {
        video_memory[i].character = ' ';
        video_memory[i].color = color;
    }
}

void vga::write(u8x2 position, string text, ColorPair color) {
    u8x2 current_position = position;
    for (u8 character : text) {
        if (character == '\n') {
            current_position.x = position.x;
            current_position.y++;
        } else {
            auto cell_maybe = vga::cell(current_position);
            if (cell_maybe) {
                auto cell = cell_maybe.unwrap();
                cell->character = character;
                cell->color = color;
            }
            current_position.x++;
        }
    }
}

void vga::cursor_move(u8x2 position) {
    u16 index = position.y * vga::ScreenSize.width + position.x;

    // send low byte
    io::write(0x3D4, 0x0F);
    io::write(0x3D5, (u8) (index & 0xFF));

    // send high byte
    io::write(0x3D4, 0x0E);
    io::write(0x3D5, (u8) ((index >> 8) & 0xFF));
}

void vga::cursor_visible(bool enable) {
    io::write(0x3D4, 0x0A);
    u8 val = io::read(0x3D5);
    io::write(0x3D5, enable ? val & ~0x20 : val | 0x20);
}
