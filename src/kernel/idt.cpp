#include "idt.hpp"
#include "modern.hpp"
#include "kernel/death_screen.hpp"

namespace {
    struct __attribute__ ((packed)) IdtEntry {
        u16 offset_low;  // Lower 16 bits of the function address
        u16 selector;    // Kernel Code Segment Selector (0x08)
        u8  zero;        // Always 0
        u8  type_attr;   // Flags (Present, Ring 0, etc.)
        u16 offset_high; // Upper 16 bits of the function address
    };

    struct __attribute__ ((packed)) IdtPtr {
        u16 limit;
        u32 base;
    };

    __attribute__ ((aligned(0x10)))
    static IdtEntry idt_table[256];
    static IdtPtr idt_ptr;

    void set_gate(u8 index, u32 base, u16 sel, u8 flags) {
        idt_table[index].offset_low = base & 0xFFFF;
        idt_table[index].offset_high = (base >> 16) & 0xFFFF;
        idt_table[index].selector = sel;
        idt_table[index].zero = 0;
        idt_table[index].type_attr = flags;
    }

    vector<string, 15> descriptions = {
        "Division by zero",
        "<unknown>",
        "<unknown>",
        "<unknown>",
        "<unknown>",
        "<unknown>",
        "Invalid Opcode: CPU attempted to execute garbage data",
        "<unknown>",
        "Double Fault: Error inside the error handler",
        "<unknown>",
        "<unknown>",
        "<unknown>",
        "<unknown>",
        "General Protection Fault: Permission violations",
        "Page Fault: Attempting to access invalid memory",
    };
}

extern "C" {
    void isr0();
    void isr6();
    void isr8();
    void isr13();
    void isr14();
}

void idt::init() {
    idt_ptr.limit = sizeof(IdtEntry) * 256 - 1;
    idt_ptr.base = reinterpret_cast<address>(&idt_table);

    set_gate(0, reinterpret_cast<address>(isr0), 0x08, 0x8E);
    set_gate(6, reinterpret_cast<address>(isr6), 0x08, 0x8E);
    set_gate(8, reinterpret_cast<address>(isr8), 0x08, 0x8E);
    set_gate(13, reinterpret_cast<address>(isr13), 0x08, 0x8E);
    set_gate(14, reinterpret_cast<address>(isr14), 0x08, 0x8E);

    __asm__ volatile("lidt %0" : : "m"(idt_ptr));
}

void idt::handler(idt::IsrRegisters *registers) {
    death_screen::show(descriptions.get(registers->int_no).unwrap_or("<unknown>"));
}
