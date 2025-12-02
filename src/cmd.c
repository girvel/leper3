#pragma once

#include "modern/string.h"
#include "tty.c"

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

cmd_Entry cmd_entries_base[4] = {
    {literal("split"), _split, literal("")},
    {literal("echo"), _echo, literal("")},
    {literal("clear"), _clear, literal("")},
    {literal("help"), _help, literal("display help")},
};

cmd_Entries cmd_entries = {.base = cmd_entries_base, .length = 4};

void _help(StringArray args) {
    foreach (cmd_Entry *, entry, &cmd_entries) {
        tty_write(literal("- "));
        tty_write(entry->name);
        if (entry->description.length > 0) {
            tty_write(literal(": "));
            tty_write(entry->description);
        }
        tty_write(literal("\n"));
    }
}
