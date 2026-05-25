[BITS 32]
global gdt_flush

; void gdt_flush(uint32_t gdt_ptr)
; Loads the new GDT and reloads segment registers
gdt_flush:
    mov eax, [esp + 4]     ; Get GDT pointer from stack
    lgdt [eax]             ; Load the GDT

    mov ax, 0x10           ; 0x10 = offset to data segment (entry 2)
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    jmp 0x08:.flush        ; 0x08 = offset to code segment (entry 1)
.flush:
    ret
