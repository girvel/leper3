#pragma once

#include "modern/string.h"
#include "tty.c"
#include "heap.c"
#include "clock.c"
#include "power.c"
#include "random.c"

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

cmd_Entry cmd_entries_base[8] = {
    {literal("split"), _split, literal("")},
    {literal("echo"), _echo, literal("")},
    {literal("clear"), _clear, literal("")},
    {literal("date"), _date, literal("display date/time")},
    {literal("reboot"), _reboot, literal("")},
    {literal("crash"), _crash, literal("emulate OS crash")},
    {literal("random"), _random, literal("generate a random number")},
    {literal("help"), _help, literal("display help")},
};

cmd_Entries cmd_entries = {.base = cmd_entries_base, .length = 8};

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
