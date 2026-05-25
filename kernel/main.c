/* GorkemOS Kernel - Main Entry Point */
#include "../include/types.h"
#include "../include/vga.h"
#include "../include/print.h"
#include "../include/gdt.h"
#include "../include/idt.h"
#include "../include/timer.h"
#include "../include/keyboard.h"
#include "../include/pmm.h"
#include "../include/paging.h"
#include "../include/heap.h"
#include "../include/shell.h"

#define GORKEM_OS_VERSION "0.2"
#define ASSUMED_MEM_KB    (32 * 1024)  /* Assume 32 MB */

void kernel_main(void) {
    /* Initialize GDT */
    gdt_init();

    /* Initialize VGA driver */
    vga_init();

    /* ASCII art boot logo */
    vga_set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    vga_print("   ____            _                   ___  ____\n");
    vga_print("  / ___| ___  _ __| | _____ _ __ ___  / _ \\/ ___|\n");
    vga_print(" | |  _ / _ \\| '__| |/ / _ \\ '_ ` _ \\| | | \\___ \\\n");
    vga_print(" | |_| | (_) | |  |   <  __/ | | | | | |_| |___) |\n");
    vga_print("  \\____|\\___/|_|  |_|\\_\\___|_| |_| |_|\\___/|____/\n");
    vga_newline();

    vga_set_color(COLOR_WHITE, COLOR_BLACK);

    /* Initialize subsystems */
    vga_print_color("[OK] ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    vga_print("GDT initialized.\n");

    vga_print_color("[OK] ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    vga_print("VGA driver loaded.\n");

    idt_init();
    vga_print_color("[OK] ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    vga_print("IDT initialized.\n");

    timer_init(100);
    vga_print_color("[OK] ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    vga_print("PIT timer (100 Hz).\n");

    keyboard_init();
    vga_print_color("[OK] ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    vga_print("Keyboard driver.\n");

    pmm_init(ASSUMED_MEM_KB);
    vga_print_color("[OK] ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    kprintf("PMM: %u KB free / %u KB total.\n",
            pmm_get_free_pages() * 4, pmm_get_total_pages() * 4);

    paging_init();
    vga_print_color("[OK] ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    vga_print("Paging enabled.\n");

    heap_init();
    vga_print_color("[OK] ", COLOR_LIGHT_GREEN, COLOR_BLACK);
    vga_print("Heap allocator (2 MB).\n");

    /* Launch the shell */
    shell_init();
    shell_run();

    /* Should never reach here */
    while (1) {
        __asm__ __volatile__("hlt");
    }
}
