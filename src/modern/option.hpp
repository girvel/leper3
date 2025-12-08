#pragma once
#include "assert.hpp"


namespace internal {
    template<typename T>
    struct checked_option {
        T value;
        bool has_value;

        explicit operator bool() const { return has_value; }

        T *operator ->() {
            massert(has_value && "Accessing empty checked_option via ->");
            return &value;
        }

        const T *operator ->() const {
            massert(has_value && "Accessing empty checked_option via ->");
            return &value;
        }

        T &operator *() {
            massert(has_value && "Accessing empty checked_option via *");
            return value;
        }

        const T& operator *() const {
            massert(has_value && "Accessing empty checked_option via *");
            return value;
        }
    };

    template<typename U>
    struct checked_option<U *> {
        U *value;
        bool has_value;

        explicit operator bool() const { return has_value; }

        U *operator ->() {
            massert(has_value && "Accessing empty checked_option via ->");
            return value;
        }

        const U *operator ->() const {
            massert(has_value && "Accessing empty checked_option via ->");
            return value;
        }

        U &operator *() {
            massert(has_value && "Accessing empty checked_option via *");
            return value;
        }
    };
}

struct NoneType {};
constexpr NoneType none = {};

template<typename T>
struct option {
    option(const T &value) : value(value), has_value(true) {}
    option(NoneType value) : no_value(none), has_value(false) {
        (void)value;
    }

    inline const T &unwrap() {
        if (!this->has_value) panic("Unwrapping none");
        return this->value;
    }

    inline internal::checked_option<T> check() {
        if (!this->has_value) return {.value = {}, .has_value = false};
        return {.value = this->value, .has_value = true};
    }

    operator bool() {
        return this->has_value;
    }

private:
    union {
        T value;
        NoneType no_value;
    };
    bool has_value;
};

#define try(...) ({ \
    auto __option = (__VA_ARGS__); \
    if (!__option) return none; \
    __option.unwrap(); \
})
