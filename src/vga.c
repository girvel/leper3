#include "leper3.h"

#define VGA_CELL_ADDRESS 0xb8000

vga_Cell no_cell;

vga_Cell *vga_cell(u8 x, u8 y) {
    if (x < VGA_CELL_W && y < VGA_CELL_H) {
        vga_Cell *video_memory = (vga_Cell *)VGA_CELL_ADDRESS;
        return video_memory + y * VGA_CELL_W + x;
    }

    return &no_cell;
}

void vga_write(u8 x, u8 y, const u8 *str, vga_Color color) {
    u8 current_x = x;
    u8 current_y = y;
    for (const u8 *ch = str; *ch; ch++) {
        if (*ch == '\n') {
            current_x = x;
            current_y++;
        } else {
            vga_Cell *cell = vga_cell(current_x, current_y);
            cell->character = *ch;
            cell->color = color;
            current_x++;
        }
    }
}

void vga_clear(vga_Color color) {
    vga_Cell *video_memory = (vga_Cell *)VGA_CELL_ADDRESS;

    for (usize i = 0; i < VGA_CELL_W * VGA_CELL_H; i++) {
        video_memory[i] = (vga_Cell) {.character = ' ', .color = color};
    }
}

void vga_cursor_move(u8 x, u8 y) {
    u16 index = y * VGA_CELL_W + x;

    // send low byte
    io_write_byte(0x3D4, 0x0F);
    io_write_byte(0x3D5, (u8) (index & 0xFF));

    // send high byte
    io_write_byte(0x3D4, 0x0E);
    io_write_byte(0x3D5, (u8) ((index >> 8) & 0xFF));
}

void vga_cursor(bool enable) {
    io_write_byte(0x3D4, 0x0A);
    u8 val = io_read_byte(0x3D5);
    io_write_byte(0x3D5, enable ? val & ~0x20 : val | 0x20);
}
