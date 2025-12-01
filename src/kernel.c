void run();

// NOTICE: should come first!
int main() {
    run();
    while(1);
    return 0;
}

#include "modern/string.h"
#include "modern/types.h"
#include "arena.c"
#include "vga.c"

static inline u8 io_read_byte(u16 port) {
    u8 result;
    __asm__ volatile("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

#define KB_DATA_PORT 0x60
#define KB_STATUS_PORT 0x64

u8 kb_scancode_map[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

#define KB_SCANCODE_RELEASE 0x80

u8 kb_read_scancode() {
    if (io_read_byte(KB_STATUS_PORT) & 0x01) {
        return io_read_byte(KB_DATA_PORT);
    }
    return 0;
}

u8 kb_read() {
    u8 scancode = kb_read_scancode();
    if (scancode == 0 || scancode & KB_SCANCODE_RELEASE) return 0;
    return kb_scancode_map[scancode];
}

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
    
    // vga_write(LITERAL("> "), terminal);

    // while (true) {
    //     u8 key = kb_read();
    //     if (key != 0) {
    //         vga_write((String) {.base = &key, .length = 1}, terminal);
    //     }
    // }

    vga_write(LITERAL("Finished.\n"), terminal);
}
