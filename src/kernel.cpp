void run();

extern "C" [[noreturn]] int main() {
    run();
    while (true) { __asm__ volatile ("hlt"); }
}

#include "kernel/tty.hpp"
#include "kernel/death_screen.hpp"

extern "C" void __cxa_pure_virtual() {
    death_screen::show("Vtable error: virtual method not found");
    while (true) { __asm__ volatile ("hlt"); }
}

void run() {
    tty::clear();
    tty::write("Hello, world!\n");
    vga::write({2, 4}, "SSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS", tty::Terminal);
}
