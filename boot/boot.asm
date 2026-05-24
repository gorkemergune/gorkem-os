section .text 
global _start
_start:
    ; Set up the stack
    mov ax, 0x0000
    mov ss, ax
    mov sp, 0x7C00

    ; Load the kernel (assuming it's located at 0x1000)
    mov si, kernel_msg
    call print_string

    ; Jump to the kernel entry point (0x1000)
    jmp 0x1000  

print_string:
    ; Print the string pointed to by SI
    mov ah, 0x0E  ; BIOS teletype output function
.print_loop:
    lodsb           ; Load byte at SI into AL and increment SI
    cmp al, 0       ; Check for null terminator
    je .done        ; If null terminator, we're done
    int 0x10        ; Call BIOS to print the character in AL
    jmp .print_loop ; Repeat for the next character
.done:
    ret
section .data
kernel_msg db 'Loading kernel...', 0



