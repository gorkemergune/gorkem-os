/* GorkemOS Linker Script */
/* Defines where the kernel is loaded in memory */

ENTRY(_start)

SECTIONS
{
    /* Kernel is loaded at 0x1000 by the bootloader */
    . = 0x1000;

    .text : {
        *(.text)
    }

    .rodata : {
        *(.rodata)
    }

    .data : {
        *(.data)
    }

    .bss : {
        *(.bss)
        *(COMMON)
    }
}
