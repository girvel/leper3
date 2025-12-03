#pragma once

#include "modern/string.h"
#include "tty.c"
#include "heap.c"
#include "clock.c"
#include "power.c"
#include "random.c"
#include "vga.c"

void _split(StringArray args) {
    StringArray words = args;
    words.base++;
    words.length--;

    enumerate (address, i, String *, word, &words) {
        if (i > 0) tty_write(literal("\n"));
        tty_write(*word);
    }
}

void _echo(StringArray args) {
    StringArray words = args;
    words.base++;
    words.length--;

    enumerate(address, i, String *, word, &words) {
        if (i > 0) tty_write(literal(" "));
        tty_write(*word);
    }
}

void _clear(StringArray args) {
    tty_clear();
}

void _date(StringArray args) {
    Allocator heap = heap_get_allocator();

    Time time = clock_read();

    DynamicString result = {0};
    string_format(&result, &heap, literal("%t"), &time);

    tty_write(to_fat(String, result));
    free(&heap, result);
}

void _reboot(StringArray args) {
    power_reboot();
}

void _crash_help() {
    tty_write(literal("crash zero: divide by zero\n"));
    tty_write(literal("crash invalid: execute invalid opcode\n"));
    tty_write(literal("crash gpf: trigger general protection fault\n"));
    tty_write(literal("crash null: break paging"));
}

void _crash(StringArray args) {
    if (args.length < 2) {
        _crash_help();
        return;
    }

    String arg = args.base[1];
    if (string_equal(arg, literal("zero"))) {
        volatile u8 zero = 0;
        u8 _ = 1 / zero;
        (void)_;
    } else if (string_equal(arg, literal("invalid"))) {
        __asm__ volatile("ud2");
    } else if (string_equal(arg, literal("gpf"))) {
       __asm__ volatile("mov $0xFFFF, %ax; mov %ax, %ds");
    } else if (string_equal(arg, literal("null"))) {
        *((volatile u8 *)0x0) = 0;
    } else {
        _crash_help();
    }
}

void _random(StringArray args) {
    Allocator heap = heap_get_allocator();
    i32 n;
    if (args.length == 1) {
        n = random_next();
    } else if (args.length == 2) {
        i32 arg;
        if (!string_to_signed(args.base[1], &arg)) {
            tty_write(literal("Invalid argument; should be an integer"));
            return;
        }

        n = (u32)random_next() % arg;
    } else {
        return;
    }
    DynamicString str = {0};
    string_write_signed(&str, &heap, n);
    tty_write(to_fat(String, str));
    free(&heap, str);
}

static inline i32 mod(i32 a, i32 b) {
    i32 r = a % b;
    return r < 0 ? r + b : r;
}

void _game_of_life(StringArray args) {
    const u8 w = VGA_VIDEO_MEMORY_W;
    const u8 h = VGA_VIDEO_MEMORY_H;
    const u8 alive = 0x07;
    const u8 dead = ' ';

    u8 field[w][h];

    u8_2 pos;
    for (pos.x = 0; pos.x < w; pos.x++) {
    for (pos.y = 0; pos.y < h; pos.y++) {
        field[pos.x][pos.y] = random_next() > 0 ? 0x07 : ' ';
    }}

    vga_Color color = vga_Color_fg_white | vga_Color_bg_black;

    while (true) {
        for (pos.x = 0; pos.x < w; pos.x++) {
        for (pos.y = 0; pos.y < h; pos.y++) {
            vga_Cell *cell = vga_cell(pos);
            cell->character = field[pos.x][pos.y];
            cell->color = color;
        }}

        for (pos.x = 0; pos.x < w; pos.x++) {
        for (pos.y = 0; pos.y < h; pos.y++) {
            u8 neighbours =
                (field[mod(pos.x - 1, w)][mod(pos.y - 1, h)] == alive) +
                (field[mod(pos.x - 1, w)][pos.y] == alive) +
                (field[mod(pos.x - 1, w)][mod(pos.y + 1, h)] == alive) +
                (field[pos.x][mod(pos.y - 1, h)] == alive) +
                (field[pos.x][mod(pos.y + 1, h)] == alive) +
                (field[mod(pos.x + 1, w)][mod(pos.y - 1, h)] == alive) +
                (field[mod(pos.x + 1, w)][pos.y] == alive) +
                (field[mod(pos.x + 1, w)][mod(pos.y + 1, h)] == alive);
            bool is_alive = field[pos.x][pos.y] == alive;

            field[pos.x][pos.y] =
                (is_alive && neighbours >= 2 && neighbours <= 3)
                || (!is_alive && neighbours == 3)
                    ? alive : dead;
        }}

        for (volatile address i = 0; i < 1000000; i++) {
            if (kb_read() == '\n') {
                tty_clear();
                return;
            }
        }

    }
}

void _help(StringArray);

typedef struct {
    String name;
    void (*f)(StringArray args);
    String description;
} cmd_Entry;

typedef struct {
    cmd_Entry *base;
    address length;
} cmd_Entries;

cmd_Entry cmd_entries_base[9] = {
    {literal("split"), _split, literal("")},
    {literal("echo"), _echo, literal("")},
    {literal("clear"), _clear, literal("")},
    {literal("date"), _date, literal("display date/time")},
    {literal("reboot"), _reboot, literal("")},
    {literal("crash"), _crash, literal("emulate OS crash")},
    {literal("random"), _random, literal("generate a random number")},
    {literal("gol"), _game_of_life, literal("play game of life")},
    {literal("help"), _help, literal("display help")},
};

cmd_Entries cmd_entries = {.base = cmd_entries_base, .length = 9};

void _help(StringArray args) {
    enumerate (address, i, cmd_Entry *, entry, &cmd_entries) {
        if (i > 0) tty_write(literal("\n"));
        tty_write(literal("- "));
        tty_write(entry->name);
        if (entry->description.length > 0) {
            tty_write(literal(": "));
            tty_write(entry->description);
        }
    }
}
