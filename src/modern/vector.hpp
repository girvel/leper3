#pragma once

#include "integer.hpp"

namespace internal {
    template<typename T, address Size>
    struct VectorStorage {
        union {
            T items[Size];
            struct { T x, y, z, w; };
            struct { T r, g, b, a; };
            struct { T width, height, depth, duration; };
        };
    };

    template<typename T>
    struct VectorStorage<T, 1> {
        union {
            T items[1];
            struct { T x; };
            struct { T r; };
            struct { T width; };
        };
    };

    template<typename T>
    struct VectorStorage<T, 2> {
        union {
            T items[2];
            struct { T x, y; };
            struct { T r, g; };
            struct { T width, height; };
        };
    };

    template<typename T>
    struct VectorStorage<T, 3> {
        union {
            T items[3];
            struct { T x, y, z; };
            struct { T r, g, b; };
            struct { T width, height, depth; };
        };
    };

}

template<typename T, address Size>
struct Vector : public internal::VectorStorage<T, Size> {
    constexpr Vector() : internal::VectorStorage<T, Size>{ { {0} } } {}

    template <typename... Args>
    constexpr Vector(Args... args) : internal::VectorStorage<T, Size>{ { {static_cast<T>(args)...} } } {
        static_assert(sizeof...(args) <= Size, "Too many arguments");
    }

    constexpr T &operator[](address index) { return this->items[index]; }
    constexpr const T &operator[](address index) const { return this->items[index]; }
};
