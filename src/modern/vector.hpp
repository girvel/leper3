#pragma once

#include "integer.hpp"

template<typename T, address Size>
struct Vector {
    T items[Size];

    constexpr Vector() : items{} {}

    template <typename... Args>
    constexpr Vector(Args... args) : items{static_cast<T>(args)...} {}

    constexpr T &operator[](address index) { return items[index]; }
    constexpr const T &operator[](address index) const { return items[index]; }
};
