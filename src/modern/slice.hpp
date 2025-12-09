#pragma once

#include "integer.hpp"
#include "assert.hpp"
#include "math.hpp"

template<typename T>
struct slice {
    using Base = T;

    T *base;
    address size;

    slice(T *base, address size) : base(base), size(size) {}
    slice(T *base) : base(base), size(1) {}
    slice(): base(nullptr), size(0) {}

    T &operator[](address index) {
        massert(index < this->size && "slice index overflow");
        return this->base[index];
    }

    const T &operator[](address index) const {
        massert(index < this->size && "slice index overflow");
        return this->base[index];
    }

    T *begin() { return base; }
    T *end() { return base + size; }

    const T *begin() const { return base; }
    const T *end() const { return base + size; }

    void copy(slice<T> destination) const {
        massert(destination.size >= this->size && "Attempt to copy into smaller slice");
        for (address i = 0; i < this->size; i++) {
            destination[i] = this->base[i];
        }
    }

    void reverse() {
        address limit = this->size / 2;
        for (address i = 0; i < limit; i++) {
            swap(this->base[i], this->base[this->size - 1 - i]);
        }
    }
};
