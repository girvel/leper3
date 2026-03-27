#pragma once

// Set all pages as unused; do not call this
void page_reset();

void *page_allocate();

void page_free(void *page);

#define PAGE_BASE 0x100000
#define PAGE_SIZE 4096
#define PAGE_PAGES 256
