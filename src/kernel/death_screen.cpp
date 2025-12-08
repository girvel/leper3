#include "kernel/death_screen.hpp"
#include "kernel/vga.hpp"

void death_screen::show(string message) {
    vga::ColorPair red = vga::make_color(vga::Color::White, vga::Color::Red);
    vga::clear(red);
    vga::write({2, 1}, "RED DEATH SCREEN", red);
    vga::write({2, 2}, message, red);

    // TODO keyboard
    vga::write({2, vga::ScreenSize.height - 2}, "Press [Enter] to reboot...", red);
}
