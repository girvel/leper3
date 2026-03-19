#pragma once
#include "assert.hpp"
#include "math.hpp"
#include "option.hpp"


namespace modern {
    template<typename Derived, typename T>
    struct SliceTrait {
        constexpr Derived &self() {
            return *static_cast<Derived *>(this);
        }

        constexpr const Derived &self() const {
            return *static_cast<const Derived *>(this);
        }

        // SHARED METHODS //

        T &operator[](address index) {
            massert(index < self().len() && "slice index overflow");
            return self().ptr()[index];
        }

        const T &operator[](address index) const {
            massert(index < self().len() && "slice index overflow");
            return self().ptr()[index];
        }

        option<T *> at(address index) {
            if (index >= self().len()) return none;
            return &self().ptr()[index];
        }

        option<const T *> at(address index) const {
            if (index >= self().len()) return none;
            return &self().ptr()[index];
        }

        option<T> get(address index) const {
            if (index >= self().len()) return none;
            return self().ptr()[index];
        }

        constexpr T *begin() { return self().ptr(); }
        constexpr T *end() { return self().ptr() + self().len(); }

        constexpr const T *begin() const { return self().ptr(); }
        constexpr const T *end() const { return self().ptr() + self().len(); }

        template<typename D>
        void copy(SliceTrait<D, T> destination) const {
            massert(destination.self().len() >= self().len() && "Attempt to copy into smaller slice");
            for (address i = 0; i < self().len(); i++) {
                destination[i] = self().ptr()[i];
            }
        }

        void reverse() {
            address limit = self().len() / 2;
            for (address i = 0; i < limit; i++) {
                swap(self().ptr()[i], self().ptr()[self().len() - 1 - i]);
            }
        }
    };
}
