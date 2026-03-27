void kernel_start();

// NOTICE: should come first!
void _start() {
    kernel_start();
}

#include "leper3.h"
#include "idt.c"
#include "vmm.c"

void main();

void kernel_start() {
    page_reset();
    idt_init();
    vmm_init();

    vga_Color white = vga_fg_white | vga_bg_black;
    vga_clear(white);
    vga_write(1, 1, "LEPER OS", white);
    vga_write(1, 2, "Launched, proceeding to userspace...", white);
    vga_cursor(false);

    main();

    while(1);
}
