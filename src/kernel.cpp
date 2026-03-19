void run();

extern "C" [[noreturn]] int main() {
    run();
    while (true) { __asm__ volatile ("hlt"); }
}

#include "kernel/tty.hpp"
#include "kernel/allocation.hpp"
#include "kernel/death_screen.hpp"
#include "kernel/idt.hpp"

extern "C" void __cxa_pure_virtual() {
    death_screen::show("Vtable error: virtual method not found");
    while (true) { __asm__ volatile ("hlt"); }
}

void run() {
    idt::init();

    tty::clear();

    buffer<128> buf;
    allocation::Arena arena(buf.to_slice());

    volatile u8 zero = 0;
    u8 _ = 1 / zero;

    while (true) {
        tty::write("> ");
        string command = tty::read(&arena, '\n');
        tty::write(command);
        tty::write("\n");

        tty::write(conv::from_int(arena.size, &arena));
        tty::write("\n");
    }
}
