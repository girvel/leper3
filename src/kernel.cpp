void run();

extern "C" [[noreturn]] int main() {
    run();
    while (true) { __asm__ volatile ("hlt"); }
}

#include "kernel/tty.hpp"
#include "kernel/allocation.hpp"
#include "kernel/death_screen.hpp"

extern "C" void __cxa_pure_virtual() {
    death_screen::show("Vtable error: virtual method not found");
    while (true) { __asm__ volatile ("hlt"); }
}

void run() {
    tty::clear();
    tty::write("Hello, world!\n");

    buffer<128> region;
    allocation::Arena arena(region.to_slice());

    auto str = arena.allocate<string>(3);
    str[0] = '.';
    str[1] = '.';
    str[2] = '.';
    str[3] = '.';
    tty::write(str);
    arena.deallocate(str);
}
