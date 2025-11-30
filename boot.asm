bits 16  ; 16-bit mode
org 0x7c00  ; bootloader address

start:
    mov [BOOT_DRIVE], dl

    ; --- STACK SETUP ---
    cli
    xor ax, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    ; --- DATA SEGMENT SETUP ---
    mov es, ax
    mov ds, ax

    mov si, BOOTLOADER_MSG
    call writeln

hang:
    jmp hang

; --- FUNCTIONS ---

writeln:
    call write

    mov al, 10
    int 0x10

    mov al, 13
    int 0x10

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

; disk_load:
;     ; DL: the drive
;     ; Writes to ES:BX
;     push dx
; 
;     mov ah, 0x02
;     mov al, dh
;     mov ch, 0x00
;     mov dh, 0x00
;     mov cl, 0x02
; 
;     int 0x13
 
; --- VARIABLES ---
 
BOOT_DRIVE: db 0
; DISK_ERROR_MSG: db "Disk read error!", 0
BOOTLOADER_MSG: db "Bootloading...", 0

; --- PADDING ---

times 510-($-$$) db 0
dw 0xaa55
