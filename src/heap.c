// DEVELOPMENT FROZEN, DO NOT USE

#include "modern/integer.h"
#include "modern/memory.h"
#include "modern/allocator.h"
#include "page.c"

typedef struct heap_Record {
    address size;
    bool is_free;
    struct heap_Record *next;
} heap_Record;

Fat heap_memory;
heap_Record *heap;

void heap_init() {
    heap_memory = page_allocate();
    heap = heap_memory.base;
    heap->size = heap_memory.length - sizeof(heap_Record);
    heap->is_free = true;
    heap->next = 0;
}

void _heap_free_raw(void *payload, void *base) {
    if (base == 0) return;

    heap_Record *record = (heap_Record *)(base - sizeof(heap_Record));
    record->is_free = true;
    if (record->next != 0 && record->next->is_free) {
        record->size += sizeof(heap_Record) + record->next->size;
        record->next = record->next->next;
    }
}

void *_heap_allocate_raw(void *payload, void *prev, address length) {
    _heap_free_raw(payload, prev);

    for (heap_Record *current = heap; current != 0; current = current->next) {
        if (current->is_free && current->size >= length) {
            if (current->size > length + sizeof(heap_Record)) {
                heap_Record *new_record = (heap_Record *) ((u8 *)current + sizeof(heap_Record) + length);

                new_record->size = current->size - sizeof(heap_Record);
                new_record->is_free = true;
                new_record->next = current->next;

                current->size = length;
                current->next = new_record;
            }

            current->is_free = false;
            return (u8 *)current + sizeof(heap_Record);
        }
    }

    return 0;
}

Allocator heap_get_allocator() {
    return (Allocator) {
        .allocate_raw = _heap_allocate_raw,
        .free_raw = _heap_free_raw,
    };
}

