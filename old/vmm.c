// Virtual Memory Manager

#pragma once

#include "page.c"

typedef enum {
    vmm_Flag_present = 1,
    vmm_Flag_rw = 2,
} vmm_Flag;

void vmm_init() {
    Fat directory = page_allocate();
    u32 *directory_words = directory.base;

    for (address i = 0; i < 1024; i++) {
        directory_words[i] = vmm_Flag_rw;
    }

    Fat kernel = page_allocate();
    u32 *kernel_words = kernel.base;

    for (address i = 0; i < 1024; i++) {
        kernel_words[i] = (i * 4096) | vmm_Flag_present | vmm_Flag_rw;
    }

    kernel_words[0] = 0; // first page unmapped for null

    directory_words[0] = ((address)kernel_words) | vmm_Flag_present | vmm_Flag_rw;

    __asm__ volatile("mov %0, %%cr3" : : "r"(directory_words));

    u32 cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));
}
