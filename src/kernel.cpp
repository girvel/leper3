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
#include "kernel/allocation.hpp"

void run() {
    tty::clear();
    tty::write("Hello, world!\n");

    u8 region[128];
    bytes region_bytes(region, 128);
    allocation::Arena arena(region_bytes);

    auto str = arena.allocate<string>(3);
    str[0] = '.';
    str[1] = '.';
    str[2] = '.';
    tty::write(str);
    arena.deallocate(str);
}
