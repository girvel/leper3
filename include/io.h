#pragma once
#include "primitives.h"

// Read a byte from the given port
u8 io_read_byte(u16 port);

// Write a byte to the given port
void io_write_byte(u16 port, u8 data);
