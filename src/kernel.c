void run();

// NOTICE: should come first!
int main() {
    run();
    while(1);
    return 0;
}

#include "tty.c"
#include "heap.c"
#include "modern/string.h"
#include "modern/memory.h"
#include "modern/allocator.h"

// PLAN:
// + split a string using arena + stack memory
// - use page memory

void run() {
    heap_init();
    Allocator heap = heap_get_allocator();

    tty_clear();

    // tty_write(literal("# # #    #    #### #### #### ####       ###  ###    # # #\n"));
    // tty_write(literal(" # #     #    #    #  # #    #  #      #  # #        # # \n"));
    // tty_write(literal("# # #    #    #### #  # #### #  #      #  # ####    # # #\n"));
    // tty_write(literal(" # #     #    #    ###  #    ###       #  #    #     # # \n"));
    // tty_write(literal("# # #    #### #### #    #### #  #      ###  ###     # # #\n"));

    DynamicString cmd_buffer = allocate(DynamicString, &heap, null, 128);

    while (true) {
        cmd_buffer.size = 0;
        tty_write(literal("\n> "));
        tty_read(&cmd_buffer, .end = '\n');

        String cmd_full = slice(String, cmd_buffer);
        StringArray args = string_split(cmd_full, &heap, ' ');
        String cmd = args.base[0];

        if (string_equal(cmd, literal("echo"))) {
            tty_write(string_sub(cmd_full, 5, cmd_full.length));
        } else if (string_equal(cmd, literal("split"))) {
            StringArray words = args;
            words.base++;
            words.length--;

            enumerate (address, i, String *, word, &words) {
                if (i > 0) tty_write(literal("\n"));
                tty_write(*word);
            }
        } else if (string_equal(cmd, literal("clear"))) {
            tty_clear();
        } else {
            tty_write(literal("Unknown command"));
        }
    }
}
