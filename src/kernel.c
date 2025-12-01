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

    static_region(DynamicString, buffer, 128);

    while (true) {
        buffer.size = 0;
        tty_write(literal("\n> "));
        tty_read_line(&buffer);
        tty_write(slice(String, buffer));
    }
}
