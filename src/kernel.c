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
    tty_write(literal("> "));
}
