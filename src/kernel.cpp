void run();

extern "C" int main() {
    run();
    return 0;
}

#include "kernel/vga.hpp"

void run() {
    vga::Cell *zero = vga::cell({0, 0});
    zero->character = '+';
    zero->color = vga::FgWhite | vga::BgBlue;
}
