#pragma once
#include "modern.hpp"


namespace kb {
    option<u8> read_scancode();
    option<u8> read();
    u8 read_blocking();

    namespace Scancode {
        constexpr u8
            lshift = 0x2A,
            rshift = 0x46,
            arrow_prefix = 0xE0,
            arrow_up = 0x48,
            arrow_left = 0x4B,
            arrow_right = 0x4D,
            arrow_down = 0x50,
            release = 0x80;
    };

    namespace Key {
        constexpr u8
            arrow_up = 128,
            arrow_down = 129,
            arrow_left = 130,
            arrow_right = 131;
    };
}
