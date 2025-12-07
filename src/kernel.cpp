void run();

extern "C" [[noreturn]] int main() {
    run();
    while (true) { __asm__ volatile ("hlt"); }
}

#include "kernel/vga.hpp"

void run() {
    vga::ColorPair terminal = vga::make_color(vga::Color::White, vga::Color::Blue);
    vga::clear(terminal);

    u8 content[] = "Hello, world!";
    array<u8> text = {.base = content, .capacity = 13};
    vga::write({2, 1}, text, terminal);
}
