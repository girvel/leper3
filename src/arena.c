#include "modern/memory.h"
#include "modern/allocator.h"

typedef struct {
    void *base;
    address length;
    address size;
} Arena;

void *_arena_allocate_raw(void *payload, void *prev, address length) {
    Arena *self = payload;
    if (self->size + length > self->length) {
        return 0;
    }

    void *result = self->base + self->size;
    self->size += length;
    return result;
}

void _arena_free_raw(void *payload, void *base) {}

Allocator arena_get_allocator(Arena *self) {
    return (Allocator) {
        .payload = self,
        .allocate_raw = _arena_allocate_raw,
        .free_raw = _arena_free_raw,
    };
}
