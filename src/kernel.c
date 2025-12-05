void run();

// NOTICE: should come first!
int main() {
    run();
    while(1);
    return 0;
}

#include "tty.c"
#include "heap.c"
#include "cmd.c"
#include "clock.c"
#include "idt.c"
#include "vmm.c"
#include "modern/string.h"
#include "modern/memory.h"
#include "modern/allocator.h"

void run() {
    page_init();
    idt_init();
    vmm_init();
    heap_init();
    Allocator heap = heap_get_allocator();

    tty_clear();

    tty_write(literal("\nWelcome to Leper OS!\n"));
    Time t = clock_read();
    tty_writef(literal("Today is %t"), &t);
    tty_write(literal("\n"));

    // tty_write(literal("# # #    #    #### #### #### ####       ###  ###    # # #\n"));
    // tty_write(literal(" # #     #    #    #  # #    #  #      #  # #        # # \n"));
    // tty_write(literal("# # #    #    #### #  # #### #  #      #  # ####    # # #\n"));
    // tty_write(literal(" # #     #    #    ###  #    ###       #  #    #     # # \n"));
    // tty_write(literal("# # #    #### #### #    #### #  #      ###  ###     # # #\n"));

    DynamicString cmd_buffer = allocate(DynamicString, &heap, null, 128);

    while (true) {
    redo:
        cmd_buffer.size = 0;
        tty_write(literal("\n> "));
        tty_read(&cmd_buffer, .end = '\n');

        String cmd_full = to_fat(String, cmd_buffer);
        StringArray args = string_split(cmd_full, &heap, ' ');
        if (args.length == 0) goto redo;
        String cmd = args.base[0];

        foreach (cmd_Entry *, entry, &cmd_entries) {
            if (string_equal(entry->name, cmd)) {
                entry->f(args);
                goto redo;
            }
        }

        foreach (String *, arg, &args) {
            free(&heap, *arg);
        }
        free(&heap, args);

        tty_write(literal("Unknown command"));
    }
}
