[bits 32]

global isr0
extern idt_handler

isr0:
    cli
    push 0
    push 0
    jmp isr_handler

isr_handler:
    pusha
    call idt_handler
    popa
    add esp, 8  ; magic
    iret
