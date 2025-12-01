#pragma once

#define MAX2(A, B) ({ \
    __typeof__ (A) _a = (A); \
    __typeof__ (B) _b = (B); \
    _a > _b ? _a : _b; \
})

#define MIN2(A, B) ({ \
    __typeof__ (A) _a = (A); \
    __typeof__ (B) _b = (B); \
    _a < _b ? _a : _b; \
}

#define _concat2(A, B) A##B
#define concat2(A, B) _concat2(A, B)

#define _concat3(A, B, C) A##B##C
#define concat3(A, B, C) _concat3(A, B, C)

#define _concat4(A, B, C, D) A##B##C##D
#define concat4(A, B, C, D) _concat4(A, B, C, D)
