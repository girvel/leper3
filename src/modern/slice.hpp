#pragma once

#include "integer.hpp"
#include "slice_trait.hpp"

template<typename T>
struct slice: public modern::SliceTrait<slice<T>, T> {
    using Base = T;

    T *base;
    address size;

    slice(T *base, address size) : base(base), size(size) {}
    slice(T *base) : base(base), size(1) {}
    slice(): base(nullptr), size(0) {}

    constexpr T *ptr() { return this->base; }
    constexpr const T *ptr() const { return this->base; }
    constexpr address len() const { return this->size; }
};
