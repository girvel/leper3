#include "tty.hpp"
#include "kernel/kb.hpp"

namespace {
    u8x2 pos;
}

void tty::clear() {
    vga::clear(tty::Terminal);

    for (u8 x = 0; x < vga::ScreenSize.width; x++) {
    for (u8 y = 0; y < vga::ScreenSize.height; y++) {
        vga::Cell *cell = vga::cell({x, y}).unwrap();
        if (x == 0) {
            if (y == 0) cell->character = 0xC9;
            else if (y == vga::ScreenSize.height - 1) cell->character = 0xC8;
            else cell->character = 0xBA;
        } else if (x == vga::ScreenSize.width - 1) {
            if (y == 0) cell->character = 0xBB;
            else if (y == vga::ScreenSize.height - 1) cell->character = 0xBC;
            else cell->character = 0xBA;
        } else {
            if (y == 0) cell->character = 0xCD;
            else if (y == vga::ScreenSize.height - 1) cell->character = 0xCD;
            else cell->character = ' ';
        }
    }}

    vga::write({2, 0}, "Leper OS 3.0.0-alpha.2\n", tty::Terminal);
    vga::cursor_visible(false);
    pos = {2, 1};
}

void tty::write(string str) {
    for (u8 character : str) {
        if (character == '\n') {
            pos.x = 2;
            pos.y++;
        } else if (character == '\b') {
            if (pos.x == 2) {
                pos.x = vga::ScreenSize.width - 5;
                pos.y = max(1, pos.y - 1);
            } else {
                pos.x--;
            }
            vga::cell(pos).unwrap()->character = ' ';
        } else {
            vga::cell(pos).unwrap()->character = character;
            pos.x++;
            if (pos.x >= vga::ScreenSize.width - 4) {
                pos.x = 2;
                pos.y++;
            }
        }
    }
    vga::cursor_move(pos);
}

string tty::read(allocator *alloc, u8 end) {
    vga::cursor_visible(true);

    list<u8> result(alloc);
    while (true) {
        option<u8> character_maybe = kb::read();
        if (!character_maybe) continue;
        u8 character = character_maybe.unwrap();

        if (character == '\b') {
            if (result.base.size > 0) {
                result.base.size--;
                tty::write(&character);
            }
            continue;
        }

        tty::write(&character);
        if (character == end) break;

        result.push(character);
    }

    vga::cursor_visible(false);
    return result.base;
}

