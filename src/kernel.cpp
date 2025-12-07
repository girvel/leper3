void run();

extern "C" [[noreturn]] int main() {
    run();
    while (true) { __asm__ volatile ("hlt"); }
}

#include "kernel/vga.hpp"

void run() {
    vga::Cell *zero = vga::cell({0, 0});
    zero->character = '+';
    zero->color = vga::make_color(vga::Color::White, vga::Color::Blue);
}
