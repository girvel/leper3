#pragma once

#include "slice.hpp"

namespace internal {
    inline address c_string_length(const u8 *c_string) {
        address result = 0;
        while (c_string[result] != 0) result++;
        return result;
    }
};

struct string : public slice<u8> {
    using slice<u8>::slice;

    template<typename CharType>
    string(CharType *source) : slice<u8>(
        reinterpret_cast<u8 *>(source),
        internal::c_string_length(reinterpret_cast<u8 *>(source))
    ) {}

    template<typename CharType>
    string(const CharType *source) : slice<u8>(
        const_cast<u8 *>(reinterpret_cast<const u8 *>(source)),
        internal::c_string_length(reinterpret_cast<const u8 *>(source))
    ) {}
};
