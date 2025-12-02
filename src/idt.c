// handle interrupts

#pragma once

#include "modern/integer.h"
#include "modern/memory.h"
#include "vga.c"

typedef struct {
    u16 offset_low;  // Lower 16 bits of the function address
    u16 selector;    // Kernel Code Segment Selector (0x08)
    u8  zero;        // Always 0
    u8  type_attr;   // Flags (Present, Ring 0, etc.)
    u16 offset_high; // Upper 16 bits of the function address
} __attribute__ ((packed)) idt_Entry;

typedef struct {
    u16 limit;
    u32 base;
} __attribute__ ((packed)) idt_Ptr;

__attribute__ ((aligned(0x10)))
static idt_Entry idt[256];
static idt_Ptr idt_ptr;

void _idt_set_gate(u8 index, u32 base, u16 sel, u8 flags) {
    idt[index].offset_low = base & 0xFFFF;
    idt[index].offset_high = (base >> 16) & 0xFFFF;
    idt[index].selector = sel;
    idt[index].zero = 0;
    idt[index].type_attr = flags;
}

extern void isr0();

void idt_init() {
    idt_ptr.limit = sizeof(idt_Entry) * 256 - 1;
    idt_ptr.base = (address)&idt;

    _idt_set_gate(0, (u32)isr0, 0x08, 0x8E);

    __asm__ volatile("lidt %0" : : "m"(idt_ptr));
}

void idt_handler() {
    vga_clear(vga_Color_bg_red | vga_Color_fg_white);

    while (1);
}
