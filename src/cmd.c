#pragma once

#include "modern/string.h"
#include "tty.c"
#include "heap.c"
#include "clock.c"
#include "power.c"

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
    string_format(&result, &heap, literal("20%02i-%02i-%02i %02i:%02i:%02i"), time.year, time.month, time.day, time.hours, time.minutes, time.seconds);

    tty_write(to_fat(String, result));
    free(&heap, result);
}

void _reboot(StringArray args) {
    power_reboot();
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

cmd_Entry cmd_entries_base[6] = {
    {literal("split"), _split, literal("")},
    {literal("echo"), _echo, literal("")},
    {literal("clear"), _clear, literal("")},
    {literal("date"), _date, literal("display date/time")},
    {literal("reboot"), _reboot, literal("")},
    {literal("help"), _help, literal("display help")},
};

cmd_Entries cmd_entries = {.base = cmd_entries_base, .length = 6};

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
