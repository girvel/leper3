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

    ; tricks to use video memory
    mov ax, 0xb800
    mov es, ax

    mov dl, [ds:0x1000]
    mov dh, 0x0f
    mov [es:0], dx

    ; hang
    jmp $

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
 
; --- VARIABLES ---
 
BOOT_DRIVE: db 0
DISK_ERROR_MSG: db "Disk read error!", 0
DISK_READ_MSG: db "Disk read success.", 0
BOOTLOADER_MSG: db "Bootloading...", 0

; --- PADDING ---

times 510-($-$$) db 0
dw 0xaa55
