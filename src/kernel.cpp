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

    u8 input; while (true) {
        if (auto in = kb::read().check()) {
            input = *in;
            break;
        }
    }

    string str(&input, 1);
    tty::write(str);

    // list<cmd> command(&arena);
    // while (true) {
    //     command.clear();

    //     tty::write("> ");
    //     command.push(
    // }
}
