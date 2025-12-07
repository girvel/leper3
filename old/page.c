#pragma once

#include "modern/integer.h"
#include "modern/string.h"
#include "modern/memory.h"

#define PAGE_BASE 0x100000
#define PAGE_LENGTH 4096
#define PAGE_PAGES 256

u8 page_bitmap_base[PAGE_PAGES / 8];
String page_bitmap = {.base = page_bitmap_base, .length = PAGE_PAGES / 8};

void page_init() {
    foreach (u8 *, byte, &page_bitmap) {
        *byte = 0x00;
    }
}

Fat page_allocate() {
    for (address i = 0; i < page_bitmap.length; i++) {
        u8 *byte = page_bitmap.base + i;

        if (*byte == 0xFF) continue;

        for (address bit = 0; bit < 8; bit++) {
            if (!(*byte & (1 << bit))) {
                *byte |= (1 << bit);
                return (Fat) {
                    .base = (u8 *)PAGE_BASE + (*byte * 8 + bit) * PAGE_LENGTH,
                    .length = PAGE_LENGTH,
                };
            }
        }
    }

    return null;
}

void page_free(Fat page) {
    address bit_total = ((address) page.base - PAGE_BASE) / PAGE_LENGTH;
    address byte = bit_total / 8;
    u8 bit = bit_total % 8;
    page_bitmap.base[byte] &= ~(1 << bit);
}
