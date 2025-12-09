#pragma once

#include "modern.hpp"
#include "vga.hpp"

namespace tty {
    constexpr vga::ColorPair Terminal = vga::make_color(vga::Color::White, vga::Color::Blue);

    void clear();
    void write(string str);
    string read(allocator *alloc, u8 end);
}
