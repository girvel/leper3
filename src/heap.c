// DEVELOPMENT FROZEN, DO NOT USE

#include "modern/integer.h"
#include "modern/memory.h"

typedef struct heap_Record {
    u8 *address;
    address length;
    struct heap_Record *next;
} heap_Record;

#define HEAP_RECORDS_CAPACITY 64
heap_Record heap_records[HEAP_RECORDS_CAPACITY];
address heap_records_length = 0;

#define HEAP_SIZE 128
u8 heap[HEAP_SIZE];

void *heap_allocate_raw(address length) {
    if (length == 0 || heap_records_length + 1 == HEAP_RECORDS_CAPACITY) {
        return 0;
    }

    u8 *result = heap;
    heap_Record *current = heap_records;
    while (true) {
        if (current->address - result >= length) {
            goto found;
        }

        result = current->address + current->length;

        if (current->next == 0) {
            return 0;
        }

        current = current->next;
    }

    if (heap + HEAP_SIZE - result >= length) {
        goto found;
    }

found:
    heap_records[heap_records_length] = (heap_Record) {
        .address = result,
        .next = 0,
        .length = length,
    };

    if (heap_records_length > 0) {
        // TODO preserve order
        current->next = heap_records + heap_records_length;
    }

    heap_records_length++;
    return result;
}

#define heap_allocate(TYPE, LENGTH) ({ \
    __typeof__ (LENGTH) __length = (LENGTH); \
    void *base = heap_allocate_raw(__length); \
    (TYPE) {.base = base, .length = __length}; \
})

void heap_free_raw(void *base) {}
