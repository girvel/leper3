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

    tty_draw_bg();
    tty_write(literal("Leper OS 3.0.0-alpha.1\n"));

    // tty_write(literal("# # #    #    #### #### #### ####       ###  ###    # # #\n"));
    // tty_write(literal(" # #     #    #    #  # #    #  #      #  # #        # # \n"));
    // tty_write(literal("# # #    #    #### #  # #### #  #      #  # ####    # # #\n"));
    // tty_write(literal(" # #     #    #    ###  #    ###       #  #    #     # # \n"));
    // tty_write(literal("# # #    #### #### #    #### #  #      ###  ###     # # #\n"));

    static_region(DynamicString, cmd_buffer, 128);

    while (true) {
        cmd_buffer.size = 0;
        tty_write(literal("\n> "));
        tty_read(&cmd_buffer, .end = '\n');

        String cmd = slice(String, cmd_buffer);
        if (string_equal(string_sub(cmd, 0, 4), literal("echo"))) {
            tty_write(string_sub(cmd, 5, cmd.length));
        } else if (string_starts_with(cmd, literal("split"))) {
            Allocator heap = heap_get_allocator();

            Strings result = allocate(Strings, &heap, null, 128);
            DynamicString split_buffer = {0};

            foreach (u8 *, character, &cmd) {
                bool push = false;
                if (*character == ' ') {
                    push = true;
                } else {
                    da_append(&split_buffer, &heap, *character);
                }

                if (split_buffer.size > 0 && (push || character == cmd.base + cmd.length - 1)) {
                    String next = allocate(String, &heap, null, split_buffer.size);
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
