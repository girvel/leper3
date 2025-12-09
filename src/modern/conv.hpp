#pragma once
#include "string.hpp"
#include "concepts.hpp"
#include "allocator.hpp"
#include "list.hpp"

namespace conv {
    template<Integral Int>
    string from_int(Int n, allocator *alloc) {
        list<u8> builder(alloc);
        do {
            builder.push('0' + n % 10);
            n /= 10;
        } while (n != 0);
        builder.base.reverse();
        return builder.base;
    }
}
