#include "modern.hpp"
#include "kernel/death_screen.hpp"

void panic(const char *message) {
    death_screen::show(message);
}
