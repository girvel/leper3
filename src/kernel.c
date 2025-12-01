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
        tty_read(&buffer, .end = '\n');

        String cmd = slice(String, buffer);
        if (string_equal(string_sub(cmd, 0, 4), literal("echo"))) {
            tty_write(string_sub(cmd, 5, cmd.length));
        } else {
            tty_write(literal("Unknown command"));
        }
    }
}
