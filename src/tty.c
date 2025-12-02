#pragma once

#include "vga.c"
#include "kb.c"
#include "modern/integer.h"


const vga_Color terminal = vga_Color_bg_blue | vga_Color_fg_white;
u8_2 pos = {2, 1};

void tty_clear() {
    vga_clear(terminal);

    for (u8 x = 0; x < VGA_VIDEO_MEMORY_W; x++) {
    for (u8 y = 0; y < VGA_VIDEO_MEMORY_H; y++) {
        vga_Cell *cell = vga_cell((u8_2) {x, y});
        if (x == 0) {
            if (y == 0) cell->character = 0xC9;
            else if (y == VGA_VIDEO_MEMORY_H - 1) cell->character = 0xC8;
            else cell->character = 0xBA;
        } else if (x == VGA_VIDEO_MEMORY_W - 1) {
            if (y == 0) cell->character = 0xBB;
            else if (y == VGA_VIDEO_MEMORY_H - 1) cell->character = 0xBC;
            else cell->character = 0xBA;
        } else {
            if (y == 0) cell->character = 0xCD;
            else if (y == VGA_VIDEO_MEMORY_H - 1) cell->character = 0xCD;
            else cell->character = ' ';
        }
    }}

    vga_write((u8_2) {2, 0}, literal("Leper OS 3.0.0-alpha.1\n"), terminal);
    vga_cursor(false);
    pos = (u8_2) {2, 1};
}

void tty_write(String str) {
    foreach (u8 *, character, &str) {
        if (*character == '\n') {
            pos.x = 2;
            pos.y++;
        } else if (*character == '\b') {
            if (pos.x == 2) {
                pos.x = VGA_VIDEO_MEMORY_W - 5;
                pos.y = MAX2(1, pos.y - 1);
            } else {
                pos.x--;
            }
            vga_cell(pos)->character = ' ';
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
        if (character == '\b') {
            if (result->size > 0) {
                result->size--;
                tty_write((String) {.base = &character, .length = 1});
            }
            continue;
        }

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
