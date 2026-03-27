#include "page.h"
#include "primitives.h"

static u8 page_bitmap[PAGE_PAGES / 8];

void page_reset() {
    for (u8 *byte = page_bitmap; byte < page_bitmap + LEN(page_bitmap); byte++) {
        *byte = 0x00;
    }
}

void *page_allocate() {
    for (u8 *byte = page_bitmap; byte < page_bitmap + LEN(page_bitmap); byte++) {
        if (*byte == 0xFF) continue;
        for (u8 bit = 0; bit < 8; bit++) {
            u8 mask = 1 << bit;
            if (*byte & mask) continue;
            *byte |= mask;
            return (void *)PAGE_BASE + (*byte * 8 + bit) * PAGE_SIZE;
        }
    }
    return 0;
}

void page_free(void *page) {
    address bit_total = ((address) page - PAGE_BASE) / PAGE_SIZE;
    address byte = bit_total / 8;
    u8 bit = bit_total % 8;
    page_bitmap[byte] &= ~(1 << bit);
}
