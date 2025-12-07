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
struct vector : public internal::VectorStorage<T, Size> {
    constexpr vector() : internal::VectorStorage<T, Size>{ { {0} } } {}

    template<typename... Args>
    constexpr vector(Args... args) : internal::VectorStorage<T, Size>{ { {static_cast<T>(args)...} } } {
        static_assert(sizeof...(args) <= Size, "Too many arguments");
    }

    constexpr T &operator[](address index) { return this->items[index]; }
    constexpr const T &operator[](address index) const { return this->items[index]; }
};

using u8x2 = vector<u8, 2>;
using u8x3 = vector<u8, 3>;
using u8x4 = vector<u8, 4>;

using u16x2 = vector<u16, 2>;
using u16x3 = vector<u16, 3>;
using u16x4 = vector<u16, 4>;

using u32x2 = vector<u32, 2>;
using u32x3 = vector<u32, 3>;
using u32x4 = vector<u32, 4>;

using u64x2 = vector<u64, 2>;
using u64x3 = vector<u64, 3>;
using u64x4 = vector<u64, 4>;

using i8x2 = vector<i8, 2>;
using i8x3 = vector<i8, 3>;
using i8x4 = vector<i8, 4>;

using i16x2 = vector<i16, 2>;
using i16x3 = vector<i16, 3>;
using i16x4 = vector<i16, 4>;

using i32x2 = vector<i32, 2>;
using i32x3 = vector<i32, 3>;
using i32x4 = vector<i32, 4>;

using i64x2 = vector<i64, 2>;
using i64x3 = vector<i64, 3>;
using i64x4 = vector<i64, 4>;
