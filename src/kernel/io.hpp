#pragma once

#include "modern.hpp"

namespace io {
    inline u8 read(u16 port) {
        u8 result;
        __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
        return result;
    }

    inline void write(u16 port, u8 data) {
        __asm__ volatile("outb %0, %1" : : "a"(data), "Nd"(port));
    }
}
