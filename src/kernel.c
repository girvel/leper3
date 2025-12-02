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

typedef struct {
    String *base;
    address length;
    address size;
} Strings;

typedef struct {
    String *base;
    address length;
} StringArray;

#define da_append(ARRAY, ALLOCATOR, ELEMENT) do { \
    __typeof__ (ARRAY) da_append__array = (ARRAY); \
    __typeof__ (ALLOCATOR) da_append__allocator = (ALLOCATOR); \
    __typeof__ (ELEMENT) da_append__element = (ELEMENT); \
    if (da_append__array->size >= da_append__array->length) { \
        __typeof__ (*da_append__array) __new_array = allocate( \
            __typeof__ (*da_append__array), da_append__allocator, *da_append__array, \
            da_append__array->length == 0 ? 16 : da_append__array->length * 2 \
        ); \
        copy(*da_append__array, __new_array); \
        da_append__array->base = __new_array.base; \
        da_append__array->length = __new_array.length; \
    } \
    da_append__array->base[da_append__array->size++] = da_append__element; \
} while (0)

void run() {
    heap_init();
    Allocator heap = heap_get_allocator();

    tty_draw_bg();

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

        String cmd = slice(String, cmd_buffer);
        if (string_starts_with(cmd, literal("echo"))) {
            tty_write(string_sub(cmd, 5, cmd.length));
        } else if (string_starts_with(cmd, literal("split"))) {
            Strings result = {0};
            DynamicString current_word = {0};

            foreach (u8 *, character, &cmd) {
                bool push = false;
                if (*character == ' ') {
                    push = true;
                } else {
                    da_append(&current_word, &heap, *character);
                }

                if (current_word.size > 0 && (push || character == cmd.base + cmd.length - 1)) {
                    da_append(&result, &heap, slice(String, current_word));
                    current_word = (DynamicString) {0};
                }
            }

            StringArray words = slice(StringArray, result);
            enumerate (address, i, String *, word, &words) {
                if (i > 0) tty_write(literal("\n"));
                tty_write(*word);
            }
        } else {
            tty_write(literal("Unknown command"));
        }
    }
}
