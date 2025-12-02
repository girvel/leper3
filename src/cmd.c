#pragma once

#include "modern/string.h"
#include "tty.c"
#include "heap.c"
#include "clock.c"

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

    clock_Time time = clock_read();

    DynamicString result = {0};
    append_many(&result, &heap, literal("20"));
    append(&result, &heap, '2');
    append(&result, &heap, '0');
    string_write_signed(&result, &heap, time.year);
    append(&result, &heap, '-');
    string_write_signed(&result, &heap, time.month);
    append(&result, &heap, '-');
    string_write_signed(&result, &heap, time.day);
    append(&result, &heap, ' ');
    string_write_signed(&result, &heap, time.hours);
    append(&result, &heap, ':');
    string_write_signed(&result, &heap, time.minutes);
    append(&result, &heap, ':');
    string_write_signed(&result, &heap, time.seconds);

    tty_write(slice(String, result));
    free(&heap, result);
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

cmd_Entry cmd_entries_base[5] = {
    {literal("split"), _split, literal("")},
    {literal("echo"), _echo, literal("")},
    {literal("clear"), _clear, literal("")},
    {literal("date"), _date, literal("display date/time")},
    {literal("help"), _help, literal("display help")},
};

cmd_Entries cmd_entries = {.base = cmd_entries_base, .length = 5};

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
