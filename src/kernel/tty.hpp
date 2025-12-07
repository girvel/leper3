#pragma once

#include "vga.hpp"

namespace tty {
    constexpr vga::ColorPair Terminal = vga::make_color(vga::Color::White, vga::Color::Blue);

    void clear();
}
