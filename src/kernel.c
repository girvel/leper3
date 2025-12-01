void run();

// NOTICE: should come first!
int main() {
    run();
    while(1);
    return 0;
}

#include "modern/string.h"
#include "modern/integer.h"
#include "arena.c"
#include "vga.c"
#include "kb.c"

// TODO static region
u8 __region_base[128];
const Fat region = {.length = 128, .base = __region_base};

void run() {
    vga_clear(vga_Color_bg_blue);

    vga_Color terminal = vga_Color_bg_blue | vga_Color_fg_white;
    vga_write(LITERAL("Leper OS 3.0.0-alpha.1\n"), terminal);

    Arena a = arena_new(region);
    String str = arena_allocate(String, &a, 128);
    for (address i = 0; i < str.length; i++) {
        str.base[i] = 'A';
    }
    vga_write(str, terminal);
    vga_write(LITERAL("\n"), terminal);

    String str2 = arena_allocate(String, &a, 128);
    if (is_null(str2)) {
        vga_write(LITERAL("It's null!\n"), terminal);
    }

    a.length = 0;
    str2 = arena_allocate(String, &a, 128);
    if (!is_null(str2)) {
        vga_write(LITERAL("It's not null!\n"), terminal);
    }
    
    vga_write(LITERAL("> "), terminal);

    while (true) {
        u8 key = kb_read();
        if (key != 0) {
            vga_write((String) {.base = &key, .length = 1}, terminal);
        }
    }

    vga_write(LITERAL("Finished.\n"), terminal);
}
