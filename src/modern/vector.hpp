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

using u8x2 = Vector<u8, 2>;
using u8x3 = Vector<u8, 3>;
using u8x4 = Vector<u8, 4>;

using u16x2 = Vector<u16, 2>;
using u16x3 = Vector<u16, 3>;
using u16x4 = Vector<u16, 4>;

using u32x2 = Vector<u32, 2>;
using u32x3 = Vector<u32, 3>;
using u32x4 = Vector<u32, 4>;

using u64x2 = Vector<u64, 2>;
using u64x3 = Vector<u64, 3>;
using u64x4 = Vector<u64, 4>;

using i8x2 = Vector<i8, 2>;
using i8x3 = Vector<i8, 3>;
using i8x4 = Vector<i8, 4>;

using i16x2 = Vector<i16, 2>;
using i16x3 = Vector<i16, 3>;
using i16x4 = Vector<i16, 4>;

using i32x2 = Vector<i32, 2>;
using i32x3 = Vector<i32, 3>;
using i32x4 = Vector<i32, 4>;

using i64x2 = Vector<i64, 2>;
using i64x3 = Vector<i64, 3>;
using i64x4 = Vector<i64, 4>;
