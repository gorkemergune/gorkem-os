; NASM x86_64 Linux
; hello.asm

section .data
    message db "Hello, World!", 10
    length equ $ - message

section .text
    global _start

_start:
    ; write(stdout, message, length)
    mov rax, 1          ; syscall: write
    mov rdi, 1          ; file descriptor: stdout
    mov rsi, message    ; message address
    mov rdx, length     ; message length
    syscall

    ; exit(0)
    mov rax, 60         ; syscall: exit
    xor rdi, rdi        ; status code 0
    syscall