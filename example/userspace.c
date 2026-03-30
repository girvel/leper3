#include "leper3.h"

void main() {
    vga_Color white = vga_fg_white | vga_bg_black;
    vga_clear(white);

    u8 x = 0; u8 y = 0;
    while (true) {
        u8 ch = kb_read().character;
        switch (ch) {
        case 0: continue;
        case '\n':
            x = 0;
            y++;
            break;
        default:
            vga_cell(x, y)->character = ch;
            x++;
            if (x >= VGA_CELL_W) {
                x = 0;
                y++;
            }
            break;
        }
    }
    vga_write(1, 1, "Hello world!", white);
}
