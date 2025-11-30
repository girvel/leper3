void run();

// NOTICE: should come first!
int main() {
    run();
    while(1);
    return 0;
}

#include "modern/string.h"
#include "modern/types.h"
#include "heap.c"
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

void run() {
    vga_clear(vga_Color_bg_blue);

    vga_Color terminal = vga_Color_bg_blue | vga_Color_fg_white;
    vga_write(LITERAL("Leper OS 3.0.0-alpha.1\n"), terminal);
    
    String str = (String) {.base = heap_allocate(128), .length = 128};  // TODO fat pointers always?
    for (address i = 0; i < str.length; i++) {
        str.base[i] = 'A';
    }
    vga_write(str, terminal);
    vga_write(LITERAL("\n"), terminal);

    String str2 = (String) {.base = heap_allocate(128), .length = 128};  // TODO fat pointers always?
    if (str2.base == null) {
        vga_write(LITERAL("It's null!\n"), terminal);
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
