#pragma once
#include "modern.hpp"

namespace allocation {
    struct Arena : public allocator {
        bytes base;
        address size;

        Arena(bytes base) : base(base), size(0) {};

        void *allocate_raw(address bytes) override;
        void deallocate_raw(void *_) override;
    };
}
