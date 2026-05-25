[BITS 32]
[EXTERN kernel_main]

; Kernel entry point - called by the bootloader
; This sets up the C environment and calls kernel_main()
global _start
_start:
    call kernel_main
    jmp $               ; Hang if kernel_main returns
