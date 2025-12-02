void run();

// NOTICE: should come first!
int main() {
    run();
    while(1);
    return 0;
}

#include "tty.c"
#include "arena.c"
#include "modern/string.h"
#include "modern/memory.h"

// PLAN:
// - split a string using arena + stack memory
// - use page memory

typedef struct {
    String *base;
    address length;
    address size;
} Strings;

typedef struct {
    String *base;
    address length;
} StringArray;

void run() {
    tty_draw_bg();
    tty_write(literal("Leper OS 3.0.0-alpha.1\n"));

    static_region(DynamicString, cmd_buffer, 128);

    while (true) {
        cmd_buffer.size = 0;
        tty_write(literal("\n> "));
        tty_read(&cmd_buffer, .end = '\n');

        String cmd = slice(String, cmd_buffer);
        if (string_equal(string_sub(cmd, 0, 4), literal("echo"))) {
            tty_write(string_sub(cmd, 5, cmd.length));
        } else if (string_starts_with(cmd, literal("split"))) {
            static_region(String, page, 512);
            Arena arena = fat_cast(Arena, page);

            Strings result = arena_allocate(Strings, &arena, 128);
            static_region(DynamicString, split_buffer, 128);

            foreach (u8 *, character, &cmd) {
                bool push = false;
                if (*character == ' ') {
                    push = true;
                } else {
                    split_buffer.base[split_buffer.size] = *character;
                    // TODO handle overflow?
                    split_buffer.size++;
                }

                if (split_buffer.size > 0 && (push || character == cmd.base + cmd.length - 1)) {
                    String next = arena_allocate(String, &arena, split_buffer.size);
                    copy(split_buffer, next);
                    result.base[result.size] = next;
                    result.size++;
                    split_buffer.size = 0;
                }
            }

            StringArray words = slice(StringArray, result);
            foreach (String *, word, &words) {
                tty_write(*word);
                tty_write(literal("\n"));
            }
        } else {
            tty_write(literal("Unknown command"));
        }
    }
}
