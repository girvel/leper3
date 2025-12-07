void run();

extern "C" [[noreturn]] int main() {
    run();
    while (true) { __asm__ volatile ("hlt"); }
}

#include "kernel/tty.hpp"

void run() {
    tty::clear();

    tty::write("Hello, world!");
}
