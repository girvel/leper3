#pragma once

#include "io.c"

void power_reboot() {
    while (io_read_byte(0x64) & 0x02);
    io_write_byte(0x64, 0xFE);
    while (1) __asm__ volatile("hlt");
}
