#pragma once

#include "modern/integer.h"

i32 _random_last = 0;

i32 random_next() {
    _random_last = _random_last * 1103515245 + 12345;
    return _random_last;
}
