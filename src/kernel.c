void run();

// NOTICE: should come first!
int main() {
    run();
    while(1);
    return 0;
}

#include "modern/string.h"
#include "modern/integer.h"
#include "arena.c"
#include "vga.c"
#include "kb.c"

void run() {
    vga_clear(vga_Color_bg_blue | vga_Color_fg_white);

    vga_Color terminal = vga_Color_bg_blue | vga_Color_fg_white;
    for (u8 x = 0; x < VGA_VIDEO_MEMORY_W; x++) {
        vga_Cell *upper = vga_cell((u8_2) {x, 0});
        vga_Cell *lower = vga_cell((u8_2) {x, VGA_VIDEO_MEMORY_H - 1});
        if (x == 0) {
            upper->character = 0xC9;
            lower->character = 0xC8;
        } else if (x == VGA_VIDEO_MEMORY_W - 1) {
            upper->character = 0xBB;
            lower->character = 0xBC;
        } else {
            upper->character = 0xCD;
            lower->character = 0xCD;
        }
    }
    for (u8 y = 1; y < VGA_VIDEO_MEMORY_H - 1; y++) {
        vga_Cell *left = vga_cell((u8_2) {0, y});
        vga_Cell *right = vga_cell((u8_2) {VGA_VIDEO_MEMORY_W - 1, y});
        left->character = 0xBA;
        right->character = 0xBA;
    }
    vga_write((u8_2) {2, 1}, LITERAL("Leper OS 3.0.0-alpha.1\n"), terminal);
}
