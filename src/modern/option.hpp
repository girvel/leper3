#pragma once

#include "assert.hpp"

struct NoneType {};

constexpr NoneType none = {};

template<typename T>
struct option {
    option(const T &value) : value(value), contains_value(true) {}
    option(NoneType value) : no_value(none), contains_value(false) {
        (void)value;
    }

    inline const T &unwrap() {
        if (!this->contains_value) panic("Unwrapping none");
        return this->value;
    }

    operator bool() {
        return this->contains_value;
    }

private:
    union {
        T value;
        NoneType no_value;
    };
    bool contains_value;
};
