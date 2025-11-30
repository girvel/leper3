bits 16  ; 16-bit mode
org 0x7c00  ; bootloader address

start:
    ; --- STACK SETUP ---
    cli
    xor ax, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    ; --- DATA SEGMENT SETUP ---
    mov es, ax
    mov ds, ax

    mov al, 'H'
    call print_char

    mov al, 'i'
    call print_char

hang:
    jmp hang

; --- FUNCTIONS ---

print_char:
    ; AL receives the character
    mov ah, 0x0e
    int 0x10
    ret

; --- PADDING ---

times 510-($-$$) db 0
dw 0xaa55
