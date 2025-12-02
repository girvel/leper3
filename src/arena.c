#include "modern/memory.h"

typedef struct {
    void *base;
    address length;
    address size;
} Arena;

void *arena_allocate_raw(Arena *self, address length) {
    if (self->size + length > self->length) {
        return 0;
    }

    void *result = self->base + self->size;
    self->size += length;
    return result;
}

// TODO non-1-byte lengths
#define arena_allocate(TYPE, ARENA, LENGTH) ({ \
    address __length = (LENGTH); \
    (TYPE) {.base = arena_allocate_raw((ARENA), __length), .length = __length}; \
})
