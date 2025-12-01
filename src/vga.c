#include "modern/integer.h"
#include "modern/string.h"
#include "modern/macros.h"

typedef struct __attribute__((packed)) {
    u8 character;
    u8 color;
} vga_Cell;

#define VGA_VIDEO_MEMORY_ADDRESS 0xb8000
#define VGA_VIDEO_MEMORY_W 80
#define VGA_VIDEO_MEMORY_H 25

typedef enum {
    vga_Color_fg_black = 0x0,
    vga_Color_fg_blue = 0x1,
    vga_Color_fg_green = 0x2,
    vga_Color_fg_cyan = 0x3,
    vga_Color_fg_red = 0x4,
    vga_Color_fg_magenta = 0x5,
    vga_Color_fg_brown = 0x6,
    vga_Color_fg_light_gray = 0x7,
    vga_Color_fg_dark_gray = 0x8,
    vga_Color_fg_light_blue = 0x9,
    vga_Color_fg_light_green = 0xa,
    vga_Color_fg_light_cyan = 0xb,
    vga_Color_fg_light_red = 0xc,
    vga_Color_fg_light_magenta = 0xd,
    vga_Color_fg_yellow = 0xe,
    vga_Color_fg_white = 0xf,
    vga_Color_bg_black = 0x00,
    vga_Color_bg_blue = 0x10,
    vga_Color_bg_green = 0x20,
    vga_Color_bg_cyan = 0x30,
    vga_Color_bg_red = 0x40,
    vga_Color_bg_magenta = 0x50,
    vga_Color_bg_brown = 0x60,
    vga_Color_bg_light_gray = 0x70,

    vga_Color_blink = 0x80
} __attribute__((packed)) vga_Color;

u8 x = 0;
u8 y = 0;

void vga_write(String str, vga_Color color) {
    vga_Cell *video_memory = (vga_Cell *)VGA_VIDEO_MEMORY_ADDRESS;

    for (address i = 0; i < str.length; i++) {
        u8 character = str.base[i];

        if (character == '\n') {
            x = 0;
            y++;
        } else if (character == '\b') {
            x = MAX2(0, x - 1);
            video_memory[y * VGA_VIDEO_MEMORY_W + x].character = ' ';
        } else {
            vga_Cell *cell = &video_memory[y * VGA_VIDEO_MEMORY_W + x];
            cell->character = character;
            cell->color = color;
            x++;
        }

        if (x == VGA_VIDEO_MEMORY_W) {
            x = 0;
            if (y == VGA_VIDEO_MEMORY_H) {
                y = 0;
            } else {
                y++;
            }
        }
    }
}

void vga_clear(vga_Color color) {
    vga_Cell *video_memory = (vga_Cell *)VGA_VIDEO_MEMORY_ADDRESS;

    for (address i = 0; i < VGA_VIDEO_MEMORY_W * VGA_VIDEO_MEMORY_H; i++) {
        video_memory[i] = (vga_Cell) {.character = ' ', .color = color};
    }
}
