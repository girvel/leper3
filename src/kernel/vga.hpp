#include <modern.hpp>

namespace vga {
    constexpr u8 h = 25;
    constexpr u8 w = 80;

    static constexpr u8 FgBlack = 0x0;
    static constexpr u8 FgBlue = 0x1;
    static constexpr u8 FgGreen = 0x2;
    static constexpr u8 FgCyan = 0x3;
    static constexpr u8 FgRed = 0x4;
    static constexpr u8 FgMagenta = 0x5;
    static constexpr u8 FgBrown = 0x6;
    static constexpr u8 FgLightGray = 0x7;
    static constexpr u8 FgDarkGray = 0x8;
    static constexpr u8 FgLightBlue = 0x9;
    static constexpr u8 FgLightGreen = 0xa;
    static constexpr u8 FgLightCyan = 0xb;
    static constexpr u8 FgLightRed = 0xc;
    static constexpr u8 FgLightMagenta = 0xd;
    static constexpr u8 FgYellow = 0xe;
    static constexpr u8 FgWhite = 0xf;
    static constexpr u8 BgBlack = 0x00;
    static constexpr u8 BgBlue = 0x10;
    static constexpr u8 BgGreen = 0x20;
    static constexpr u8 BgCyan = 0x30;
    static constexpr u8 BgRed = 0x40;
    static constexpr u8 BgMagenta = 0x50;
    static constexpr u8 BgBrown = 0x60;
    static constexpr u8 BgLightGray = 0x70;
    static constexpr u8 Blink = 0x80;

    struct __attribute__((packed)) Cell {
        u8 character;
        u8 color;
    };

    Cell *cell(u8 x, u8 y);
}

