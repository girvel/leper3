#pragma once
#include "primitives.h"

typedef enum {
    kb_Key_up = 128,
    kb_Key_down = 129,
    kb_Key_left = 130,
    kb_Key_right = 131,
} kb_Key;

// Reads current keyboard state;
u8 kb_read();
