// Interrupt Descriptor Table -- handles interrupts

#pragma once

#include "modern/integer.h"
#include "modern/string.h"
#include "modern/memory.h"
#include "vga.c"
#include "arena.c"
#include "kb.c"
#include "power.c"

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

typedef struct {
    // Data pushed by pusha
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;

    // Data pushed by isr0
    u32 int_no;
    u32 err_code;

    // Data pushed automatically by the CPU before jumping to ISR
    u32 eip, cs, eflags; 
} __attribute__((packed)) isr_Registers;

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
extern void isr6();
extern void isr8();
extern void isr13();
extern void isr14();

void idt_init() {
    idt_ptr.limit = sizeof(idt_Entry) * 256 - 1;
    idt_ptr.base = (address)&idt;

    _idt_set_gate(0, (u32)isr0, 0x08, 0x8E);
    _idt_set_gate(6, (u32)isr6, 0x08, 0x8E);
    _idt_set_gate(8, (u32)isr8, 0x08, 0x8E);
    _idt_set_gate(13, (u32)isr13, 0x08, 0x8E);
    _idt_set_gate(14, (u32)isr14, 0x08, 0x8E);

    __asm__ volatile("lidt %0" : : "m"(idt_ptr));
}

String _idt_interrupt_names_base[15] = {
    literal("Division by zero"),
    literal("<unknown>"),
    literal("<unknown>"),
    literal("<unknown>"),
    literal("<unknown>"),
    literal("<unknown>"),
    literal("Invalid Opcode: CPU attempted to execute garbage data"),
    literal("<unknown>"),
    literal("Double Fault: Error inside the error handler"),
    literal("<unknown>"),
    literal("<unknown>"),
    literal("<unknown>"),
    literal("<unknown>"),
    literal("General Protection Fault: Permission violations"),
    literal("Page Fault: Attempting to access invalid memory"),
};

StringArray _idt_interrupt_names = {
    .base = _idt_interrupt_names_base,
    .length = sizeof(_idt_interrupt_names_base) / sizeof(_idt_interrupt_names_base[0])
};

void idt_handler(isr_Registers *registers) {
    static_region(String, arena_base, 1024);
    Arena _arena = fat_cast(Arena, arena_base);
    Allocator arena = arena_get_allocator(&_arena);

    vga_Color red = vga_Color_bg_red | vga_Color_fg_white;
    vga_clear(red);
    vga_write((u8_2) {2, 1}, literal("RED DEATH SCREEN"), red);

    DynamicString report = {0};
    string_format(
        &report, &arena, literal("Interrupt: %i, error code: %i"),
        registers->int_no, registers->err_code
    );
    vga_write((u8_2) {2, 2}, to_fat(String, report), red);

    if (registers->int_no < _idt_interrupt_names.length) {
        vga_write((u8_2) {2, 3}, _idt_interrupt_names.base[registers->int_no], red);
    }

    vga_write((u8_2) {2, VGA_VIDEO_MEMORY_H - 2}, literal("Press [Enter] to reboot..."), red);

    while (kb_read() != '\n');
    power_reboot();
}
