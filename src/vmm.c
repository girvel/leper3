// Virtual Memory Manager

#include "leper3.h"

typedef enum {
    vmm_Flag_present = 1,
    vmm_Flag_rw = 2,
} vmm_Flag;

void vmm_init() {
    // TODO use static memory (after allowing bigger kernel)
    u32 *directory = page_allocate();

    for (usize i = 0; i < 1024; i++) {
        directory[i] = vmm_Flag_rw;
    }

    u32 *kernel = page_allocate();

    for (usize i = 0; i < 1024; i++) {
        kernel[i] = (i * 4096) | vmm_Flag_present | vmm_Flag_rw;
    }

    kernel[0] = 0; // first page unmapped for null

    directory[0] = ((usize)kernel) | vmm_Flag_present | vmm_Flag_rw;

    __asm__ volatile("mov %0, %%cr3" : : "r"(directory));

    u32 cr0;
    __asm__ volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000;
    __asm__ volatile("mov %0, %%cr0" : : "r"(cr0));
}
