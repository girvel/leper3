void run();

extern "C" [[noreturn]] int main() {
    run();
    while (true) { __asm__ volatile ("hlt"); }
}

extern "C" void __cxa_pure_virtual() {
    // TODO panic
    while (true) { __asm__ volatile ("hlt"); }
}

#include "kernel/tty.hpp"
#include "kernel/death_screen.hpp"

void run() {
    tty::clear();
    tty::write("Hello, world!\n");
    death_screen::show("Just testing.");
}
