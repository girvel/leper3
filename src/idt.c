// Interrupt Descriptor Table -- handles interrupts

#pragma once

#include "modern/integer.h"
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

static void _idt_set_gate(u8 index, u32 base, u16 sel, u8 flags) {
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

#define LEN(X) sizeof(X) / sizeof(*X)

static const char *_idt_interrupt_descriptions[] = {
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

address str_write_signed(u8 *dest, address cap, i32 integer) {
    address initial_cap = cap;
    if (cap < 2) return 0;

    if (integer < 0) {
        *dest = '-';
        dest++; cap--;
        integer *= -1;
    }

    u8 *before = dest;
    while (true) {
        if (cap == 0) return 0;
        u8 digit = integer % 10;
        integer /= 10;
        *dest = '0' + digit;
        dest++; cap--;
        if (integer == 0) break;
    }

    address len = dest - before;
    for (address i = 0; 2 * i < len; i++) {
        address j = len - 1 - i;
        u8 tmp = before[i];
        before[i] = before[j];
        before[j] = tmp;
    }

    return initial_cap - cap;
}

void str_format_args(u8 *dest, address cap, const u8 *fmt, va_list args);

void str_format(u8 *dest, address cap, const u8 *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    str_format_args(dest, cap, fmt, args);
}

void str_format_args(u8 *dest, address cap, const u8 *fmt, va_list args) {
    for (const u8 *ch = fmt; *ch && cap > 1; ch++) {
        if (*ch != '%') {
            *dest = *ch;
            dest++; cap--;
            continue;
        }

        ch++;

        switch (*ch) {
        case '\0': goto end;
        case 'i': {
            address delta = str_write_signed(dest, cap - 1, va_arg(args, i32));
            if (delta == 0) {
                *dest = '^';
                goto end;
            }
            dest += delta;
            cap -= delta;
        } break;
        default: {
            *dest = '^';
            dest++; cap--;
        } break;
        }
    }

end:
    // assert(cap >= 1);
    *dest = '\0';
    dest++; cap--;
}

void idt_handler(isr_Registers *registers) {
    vga_Color on_red = vga_Color_bg_red | vga_Color_fg_white;
    vga_clear(on_red);
    vga_write_(2, 1, "RED DEATH SCREEN", on_red);

    u8 report[128];
    str_format(report, 128, "Interrupt: %i, error code: %i", registers->int_no, registers->err_code);
    vga_write_(2, 2, report, on_red);

    if (registers->int_no < LEN(_idt_interrupt_descriptions)) {
        const u8 *description = _idt_interrupt_descriptions[registers->int_no];
        vga_write_(2, 3, description, on_red);
    }

    vga_write_(2, VGA_VIDEO_MEMORY_H - 2, "Press [Enter] to reboot...", on_red);

    while (kb_read() != '\n');
    power_reboot();
}
