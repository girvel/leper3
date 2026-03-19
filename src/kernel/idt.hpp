#pragma once
#include "modern.hpp"

namespace idt {
    struct __attribute__((packed)) IsrRegisters {
        // Data pushed by pusha
        u32 edi, esi, ebp, esp, ebx, edx, ecx, eax;

        // Data pushed by isr0
        u32 int_no;
        u32 err_code;

        // Data pushed automatically by the CPU before jumping to ISR
        u32 eip, cs, eflags; 
    };

    void init();
    void handler(IsrRegisters *registers) asm("idt_handler");
}
