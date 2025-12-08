#pragma once

#include "slice.hpp"
#include "allocator.hpp"

template<typename T>
struct list {
    slice<T> base;
    address capacity;
    allocator *alloc;

    list(allocator *alloc_) : base(), capacity(0), alloc(alloc_) {}

    void push(const T &item) {
        grow(this->base.size + 1);
        this->base[this->base.size++] = item;
    }

    void clear() {
        this->base.size = 0;
    }

    void grow(address min_capacity) {
        if (this->capacity >= min_capacity) return;
        address new_capacity = this->capacity == 0 ? 16 : this->capacity * 2;
        while (min_capacity >= new_capacity) new_capacity *= 2;  // TODO handle overflow
        grow_to_exact(new_capacity);
    }

    void grow_to_exact(address new_capacity) {
        if (this->capacity >= new_capacity) return;
        this->alloc->deallocate_slice(this->base);  // TODO reallocation optimization
        auto new_slice = this->alloc->allocate_slice<slice<T>>(new_capacity);
        // if (new_slice.base != this->base.base) {
        // }
        this->base.copy(new_slice);
        this->base.base = new_slice.base;
        this->capacity = new_slice.size;
    }
};
