#pragma once

#include "integer.hpp"
#include "slice.hpp"

struct bytes : public slice<u8> {
    using slice<u8>::slice;

    template<typename T>
    bytes(const slice<T> &other) {
        base = reinterpret_cast<u8 *>(other.base);
        capacity = other.capacity * sizeof(T);
    }
};
