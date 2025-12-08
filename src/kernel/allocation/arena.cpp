#include "arena.hpp"

void *allocation::Arena::allocate_raw(address bytes) {
    if (this->size + bytes > this->base.size) {
        return 0; // TODO optional
    }

    void *result = this->base.base + this->size;
    this->size += bytes;
    return result;
}

void allocation::Arena::deallocate_raw(void *base) {
    (void)base;
}
