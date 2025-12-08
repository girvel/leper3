#pragma once

#include "integer.hpp"

template<typename T>
concept SliceLike = requires {
    typename T::Base;
};

struct allocator {
    template <SliceLike T>
    inline T allocate_slice(address capacity) {
        return T(reinterpret_cast<T::Base *>(allocate_raw(capacity * sizeof(typename T::Base))), capacity);
    }

    template <typename T>
    inline T *allocate() {
        return reinterpret_cast<T *>(allocate_raw(sizeof(T)));
    }

    template <SliceLike T>
    inline void deallocate_slice(T ref) {
        deallocate_raw(ref.base);
    }

    template<typename T>
    inline void deallocate(T *ref) {
        deallocate_raw(ref);
    }

    virtual void *allocate_raw(address bytes) = 0;
    virtual void deallocate_raw(void *base) = 0;
};

using legacy_address = decltype(sizeof(0));

// Needed by C++
inline void *operator new(legacy_address, void *ptr) { return ptr; }
inline void operator delete(void *, void*) {}

inline void *operator new(legacy_address size, allocator &alloc) {
    return alloc.allocate_raw(size);
}

inline void operator delete(void *ptr, allocator &alloc) {
    return alloc.deallocate_raw(ptr);
}
