void run();

extern "C" int main() {
    run();
    return 0;
}

#include "kernel/vga.hpp"

void run() {
    vga::cell(0, 0)->character = '+';
}
