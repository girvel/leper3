; Interrupt Service Routine

[bits 32]

%macro ISR_NOERRCODE 1
global isr%1
isr%1:
    cli
    push 0
    push %1
    jmp isr_handler
%endmacro

%macro ISR_ERRCODE 1
global isr%1
isr%1:
    cli
    push %1
    jmp isr_handler
%endmacro

ISR_NOERRCODE 0
ISR_NOERRCODE 6
ISR_ERRCODE 8
ISR_ERRCODE 13
ISR_ERRCODE 14

extern idt_handler

isr_handler:
    pusha
    push esp ; pass the stack pointer
    call idt_handler
    pop eax ; pop it
    popa
    add esp, 8  ; reset stack
    iret
