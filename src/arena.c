#include "modern/memory.h"

typedef struct {
    void *base;
    address length;
    address capacity;
} Arena;

Arena arena_new(Fat region) {
    return (Arena) {
        .base = region.base,
        .capacity = region.length,
        .length = 0,
    };
}

void *arena_allocate_raw(Arena *self, address length) {
    if (self->length + length > self->capacity) {
        return 0;
    }

    void *result = self->base + self->length;
    self->length += length;
    return result;
}

#define arena_allocate(TYPE, ARENA, LENGTH) ({ \
    address __length = (LENGTH); \
    (TYPE) {.base = arena_allocate_raw((ARENA), __length), .length = __length}; \
})
