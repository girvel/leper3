void run();

extern "C" [[noreturn]] int main() {
    run();
    while (true) { __asm__ volatile ("hlt"); }
}

#include "kernel/tty.hpp"
#include "kernel/allocation.hpp"
#include "kernel/death_screen.hpp"
#include "kernel/kb.hpp"

extern "C" void __cxa_pure_virtual() {
    death_screen::show("Vtable error: virtual method not found");
    while (true) { __asm__ volatile ("hlt"); }
}

void run() {
    tty::clear();
    tty::write("Hello, world!\n");

    buffer<128> buf;
    allocation::Arena arena(buf.to_slice());

    list<u8> command(&arena);
    while (true) {
        u8 character = kb::read_blocking();
        if (character == '\n') break;
        command.push(character);
    }
    tty::write(command.base);

    // while (true) {
    //     command.clear();

    //     tty::write("> ");
    //     command.extend(tty::read('\n'));
    // }
}
