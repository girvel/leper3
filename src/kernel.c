void run();

// NOTICE: should come first!
int main() {
    run();
    while(1);
    return 0;
}

#include "modern/string.h"
#include "vga.c"

void run() {
    clear(vga_Color_bg_blue);
    write(LITERAL("Leper OS 3.0.0-alpha.1"), vga_Color_bg_blue | vga_Color_fg_white);
}
