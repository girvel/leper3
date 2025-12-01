#include "vga.c"
#include "kb.c"
#include "modern/integer.h"


const vga_Color terminal = vga_Color_bg_blue | vga_Color_fg_white;
u8_2 pos = {2, 1};

void tty_draw_bg() {
    vga_clear(terminal);

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

    vga_cursor(false);
}

void tty_write(String str) {
    foreach (u8 *, character, &str) {
        if (*character == '\n') {
            pos.x = 2;
            pos.y++;
        } else {
            vga_cell(pos)->character = *character;
            pos.x++;
            if (pos.x >= VGA_VIDEO_MEMORY_W - 4) {
                pos.x = 2;
                pos.y++;
            }
        }
    }
    vga_cursor_move(pos);
}

typedef struct {
    u8 end;
} tty_read_Options;

void tty_read_(DynamicString *result, tty_read_Options opts) {
    vga_cursor(true);
    while (true) {
        u8 character = kb_read();
        if (character == 0) continue;

        tty_write((String) {.base = &character, .length = 1});
        if (character == opts.end) break;

        result->base[result->size] = character;
        result->size++;

        if (result->size == result->length) {
            if (opts.end != 0) tty_write((String) {.base = &opts.end, .length = 1});
            break;
        }
    }
    vga_cursor(false);
}

#define tty_read(RESULT, ...) tty_read_((RESULT), (tty_read_Options) {__VA_ARGS__})

// TODO scroll + struct
