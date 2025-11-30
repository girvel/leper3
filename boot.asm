bits 16  ; 16-bit mode
org 0x7c00  ; bootloader address

start:
    mov ah, 0x0e
    mov al, 'X'
    int 0x10

hang:
    jmp hang

times 510-($-$$) db 0
dw 0xaa55
