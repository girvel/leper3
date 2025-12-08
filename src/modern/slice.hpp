#pragma once

#include "integer.hpp"

template<typename T>
struct slice {
    using Base = T;

    T *base;
    address capacity;

    slice(T *base, address capacity) : base(base), capacity(capacity) {}

    T &operator[](address index) {
        // TODO assert here
        return this->base[index];
    }

    const T &operator[](address index) const {
        // TODO assert here
        return this->base[index];
    }

    T *begin() { return base; }
    T *end() { return base + capacity; }

    const T *begin() const { return base; }
    const T *end() const { return base + capacity; }
};
