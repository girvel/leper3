bits 16  ; 16-bit mode
org 0x7c00  ; bootloader address

jmp short start
nop

; --- FAKE BIOS PARAMETER BLOCK (BPB) ---
; This tricks the BIOS into thinking this is a valid FAT12 filesystem.
; Without this, some BIOSes (like VAIO) reject the USB stick.

oem_name:           db "MSWIN4.1"   ; 8 bytes
bytes_per_sector:   dw 512
sectors_per_cluster:db 1
reserved_sectors:   dw 1
num_fats:           db 2
root_dir_entries:   dw 224
total_sectors_short:dw 2880
media_descriptor:   db 0xf0
sectors_per_fat:    dw 9
sectors_per_track:  dw 18
num_heads:          dw 2
hidden_sectors:     dd 0
total_sectors_long: dd 0
drive_number:       db 0x80         ; 0x00 for floppy, 0x80 for HDD
reserved:           db 0
signature:          db 0x29
vol_id:             dd 0x12345678
vol_label:          db "LEPEROS    "; 11 bytes
fs_type:            db "FAT12   "   ; 8 bytes

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
    mov dh, 50
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
    call 0x1000

    jmp $

; --- VARIABLES ---
 
BOOT_DRIVE: db 0
DISK_ERROR_MSG: db "Disk read error!", 0
DISK_READ_MSG: db "Disk read success.", 0
BOOTLOADER_MSG: db "Bootloading...", 0
KERNEL_EXITED_MSG: db "Kernel finished.", 0

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

; --- MBR PARTITION TABLE HACK ---

; 1. Check if we have space. 
%if ($ - $$) > 446
    %error "Bootloader is too big! It must be < 446 bytes to fit a Partition Table."
%endif

; 2. Pad exactly up to the partition table area
times 446-($-$$) db 0

; 3. Fake Partition Entry #1 (Marked Active)
db 0x80       ; Bit 7 set = Bootable / Active
db 0, 0, 0    ; CHS Start (unused by modern BIOS)
db 0          ; Partition Type (0 = Empty, but 0x80 status is what matters)
db 0, 0, 0    ; CHS End
dd 0          ; LBA Start
dd 0          ; LBA Length

; --- PADDING ---

times 510-($-$$) db 0
dw 0xaa55
