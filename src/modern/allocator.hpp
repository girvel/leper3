#pragma once

#include "integer.hpp"

struct allocator {
    template <typename T>
    inline T allocate(address capacity = 1) {
        return T(reinterpret_cast<T::Base *>(allocate_raw(capacity * sizeof(T))), capacity);
    }

    template <typename T>
    inline void deallocate(T ref) {
        deallocate_raw(ref.base);
    }

    virtual void *allocate_raw(address bytes) = 0;
    virtual void deallocate_raw(void *base) = 0;
};
