#pragma once

[[noreturn]] void panic(const char *message);

#define massert(EXPRESSION) if (!(EXPRESSION)) { panic("Assertion failed: " #EXPRESSION); }
