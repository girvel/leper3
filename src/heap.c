#include "modern/types.h"

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

void *heap_allocate(address size) {
    if (size == 0 || heap_records_length + 1 == HEAP_RECORDS_CAPACITY) {
        return null;
    }

    u8 *result = heap;
    heap_Record *current = heap_records;
    while (true) {
        if (current->address - result >= size) {
            goto found;
        }

        result = current->address + current->length;

        if (current->next == null) {
            return null;
        }

        current = current->next;
    }

    if (heap + HEAP_SIZE - result >= size) {
        goto found;
    }

found:
    heap_records[heap_records_length] = (heap_Record) {
        .address = result,
        .next = null,
        .length = size,
    };
    if (heap_records_length > 0) {
        current->next = heap_records + heap_records_length;
    }
    heap_records_length++;

    return result;
}

void heap_free(void *base) {}
