[BITS 16]
[ORG 0x7C00]

; ============================================
; GorkemOS Bootloader
; ============================================
; BIOS loads this to 0x7C00 (first 512 bytes of disk)
; We print a message, load kernel from disk,
; switch to 32-bit protected mode, then jump to kernel.

KERNEL_OFFSET equ 0x1000   ; Where we load the kernel in memory
KERNEL_SECTORS equ 30      ; How many sectors to load (15 KB)

start:
    ; Set up segment registers and stack
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00          ; Stack grows downward from 0x7C00

    mov [BOOT_DRIVE], dl    ; BIOS passes boot drive in DL, save it

    ; Print boot message
    mov si, MSG_BOOT
    call print_string_16

    ; Load kernel from disk into memory at KERNEL_OFFSET
    call load_kernel

    ; Print message before switching to protected mode
    mov si, MSG_PMODE
    call print_string_16

    ; Switch to 32-bit protected mode
    call switch_to_pm

    jmp $                   ; Should never reach here

; ============================================
; load_kernel - Load kernel sectors from disk
; ============================================
load_kernel:
    mov si, MSG_LOAD
    call print_string_16

    mov ah, 0x02            ; BIOS read sectors function
    mov al, KERNEL_SECTORS  ; Number of sectors to read
    mov ch, 0               ; Cylinder 0
    mov cl, 2               ; Start from sector 2 (sector 1 = bootloader)
    mov dh, 0               ; Head 0
    mov dl, [BOOT_DRIVE]    ; Drive number
    mov bx, KERNEL_OFFSET   ; ES:BX = destination (0x0000:0x1000)

    int 0x13                ; BIOS disk interrupt
    jc disk_error           ; Jump if carry flag set (error)

    cmp al, KERNEL_SECTORS  ; Check if we read the expected sectors
    jne disk_error

    mov si, MSG_DONE
    call print_string_16
    ret

disk_error:
    mov si, MSG_DISK_ERR
    call print_string_16
    jmp $                   ; Hang on error

; ============================================
; print_string_16 - Print null-terminated string (16-bit real mode)
; ============================================
print_string_16:
    pusha
.loop:
    lodsb
    cmp al, 0
    je .done
    mov ah, 0x0E            ; BIOS teletype output
    mov bh, 0               ; Page 0
    int 0x10
    jmp .loop
.done:
    popa
    ret

; ============================================
; GDT - Global Descriptor Table
; ============================================
gdt_start:

gdt_null:                   ; Mandatory null descriptor (8 bytes)
    dd 0x0
    dd 0x0

gdt_code:                   ; Code segment descriptor
    dw 0xFFFF               ; Limit (bits 0-15)
    dw 0x0                  ; Base (bits 0-15)
    db 0x0                  ; Base (bits 16-23)
    db 10011010b            ; Access byte: present, ring 0, code, executable, readable
    db 11001111b            ; Flags + Limit (bits 16-19): 4KB granularity, 32-bit
    db 0x0                  ; Base (bits 24-31)

gdt_data:                   ; Data segment descriptor
    dw 0xFFFF               ; Limit (bits 0-15)
    dw 0x0                  ; Base (bits 0-15)
    db 0x0                  ; Base (bits 16-23)
    db 10010010b            ; Access byte: present, ring 0, data, writable
    db 11001111b            ; Flags + Limit (bits 16-19): 4KB granularity, 32-bit
    db 0x0                  ; Base (bits 24-31)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1   ; GDT size (limit)
    dd gdt_start                  ; GDT address

CODE_SEG equ gdt_code - gdt_start  ; 0x08
DATA_SEG equ gdt_data - gdt_start  ; 0x10

; ============================================
; switch_to_pm - Switch from 16-bit real mode to 32-bit protected mode
; ============================================
switch_to_pm:
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load GDT

    mov eax, cr0
    or eax, 0x1             ; Set PE (Protection Enable) bit
    mov cr0, eax

    jmp CODE_SEG:init_pm    ; Far jump to flush CPU pipeline

[BITS 32]
; ============================================
; init_pm - Initialize 32-bit protected mode
; ============================================
init_pm:
    mov ax, DATA_SEG        ; Set all data segment registers
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    mov ebp, 0x90000        ; Set up stack at a safe location
    mov esp, ebp

    call KERNEL_OFFSET      ; Jump to kernel!

    jmp $                   ; Should never reach here

; ============================================
; Data
; ============================================
[BITS 16]
BOOT_DRIVE:     db 0
MSG_BOOT:       db 'Booting GorkemOS...', 13, 10, 0
MSG_LOAD:       db 'Loading kernel from disk...', 13, 10, 0
MSG_DONE:       db 'Kernel loaded.', 13, 10, 0
MSG_PMODE:      db 'Switching to protected mode...', 13, 10, 0
MSG_DISK_ERR:   db 'Disk read error!', 13, 10, 0

; ============================================
; Boot signature
; ============================================
times 510 - ($ - $$) db 0   ; Pad to 510 bytes
dw 0xAA55                   ; MBR magic number
