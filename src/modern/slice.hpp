#pragma once

#include "integer.hpp"
#include "assert.hpp"

template<typename T>
struct slice {
    using Base = T;

    T *base;
    address size;

    slice(T *base, address size) : base(base), size(size) {}

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
};
