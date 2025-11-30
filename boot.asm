bits 16  ; 16-bit mode
org 0x7c00  ; bootloader address

start:
    mov [BOOT_DRIVE], dl

    ; stack setup
    cli
    xor ax, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    ; data segment setup
    mov es, ax
    mov ds, ax

    ; bootloader message
    mov si, BOOTLOADER_MSG
    call writeln

    ; reading from disk
    mov bx, 0x1000
    mov dh, 1
    mov dl, [BOOT_DRIVE]
    call disk_load

    call switch_to_pm

; --- FUNCTIONS ---

writeln:
    call write

    mov al, 10
    int 0x10

    mov al, 13
    int 0x10

    ret

write:  ; si: string pointer
    mov ah, 0x0E  ; BIOS teletype function
.loop:
    lodsb
    test al, al
    jz .done
    int 0x10
    jmp .loop
.done:
    ret

disk_load:
    ; DL: the drive
    ; Writes to ES:BX
    push dx

    mov ah, 0x02
    mov al, dh
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02

    int 0x13
    jc disk_error

    pop dx
    cmp dh, al
    jne disk_error

    mov si, DISK_READ_MSG
    call writeln

    ret

disk_error:
    mov si, DISK_ERROR_MSG
    call writeln
    jmp $ ; Hang locally

switch_to_pm:
    cli

    lgdt [gdt_descriptor]

    mov eax, cr0
    or eax, 0x1
    mov cr0, eax

    jmp CODE_SEG:init_pm

[bits 32]
init_pm:
    ; update segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; move stack
    mov ebp, 0x90000
    mov esp, ebp

    ; call entrypoint
    call begin_pm

begin_pm:
    mov byte [0xb8000], 'P'
    mov byte [0xb8001], 0x0f

    jmp $

; --- VARIABLES ---
 
BOOT_DRIVE: db 0
DISK_ERROR_MSG: db "Disk read error!", 0
DISK_READ_MSG: db "Disk read success.", 0
BOOTLOADER_MSG: db "Bootloading...", 0

; --- GDT ---

gdt_start:

gdt_null:
    dd 0x0
    dd 0x0

gdt_code: ; Base=0, Limit=4GB, overlapping code and data
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

; --- PADDING ---

times 510-($-$$) db 0
dw 0xaa55
