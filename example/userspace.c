#include "leper3.h"

void main() {
    vga_Color white = vga_fg_white | vga_bg_black;
    vga_clear(white);
    vga_write(1, 1, "Hello world!", white);
}
