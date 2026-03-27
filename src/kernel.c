void main();

// NOTICE: should come first!
int _start() {
    main();
    while(1);
    return 0;
}

#include "leper3.h"
#include "idt.c"
#include "vmm.c"

void main() {
    page_reset();
    idt_init();
    vmm_init();

    vga_Color white = vga_Color_fg_white | vga_Color_bg_black;
    vga_clear(white);
    vga_write(1, 1, "Hello, world!", white);
    vga_cursor(false);
}
