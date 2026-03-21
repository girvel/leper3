#pragma once
#include "primitives.h"

typedef enum {
    kb_Key_up = 128,
    kb_Key_down = 129,
    kb_Key_left = 130,
    kb_Key_right = 131,
} kb_Key;

// Reads current keyboard state; returns 0 if nothing's pressed, else a valid character or a kb_Key;
// any kb_Key is >= 128.
u8 kb_read();
