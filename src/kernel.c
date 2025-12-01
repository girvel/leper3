void run();

// NOTICE: should come first!
int main() {
    run();
    while(1);
    return 0;
}

#include "tty.c"
#include "modern/string.h"

void run() {
    tty_draw_bg();
    tty_write(literal("Leper OS 3.0.0-alpha.1\n"));

    u8 buffer_base[128];
    String buffer = {.base = buffer_base, .length = 128};

    while (true) {
        tty_write(literal("\n> "));
        address size = tty_read_line(&buffer);
        tty_write((String) {.base = buffer.base, .length = size});
    }
}
