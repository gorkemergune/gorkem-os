/* GorkemOS - Paging (Virtual Memory) */
#include "../include/paging.h"
#include "../include/string.h"
#include "../include/idt.h"
#include "../include/vga.h"
#include "../include/print.h"

/* Page directory and page table entries are uint32_t */
/* We identity-map the first 16 MB for simplicity */

/* Aligned to 4KB boundary */
static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));
static uint32_t second_page_table[1024] __attribute__((aligned(4096)));
static uint32_t third_page_table[1024] __attribute__((aligned(4096)));
static uint32_t fourth_page_table[1024] __attribute__((aligned(4096)));

/* Page fault handler */
static void page_fault_handler(struct registers *regs) {
    /* Read the faulting address from CR2 */
    uint32_t faulting_addr;
    __asm__ __volatile__("mov %%cr2, %0" : "=r"(faulting_addr));

    vga_set_color(COLOR_WHITE, COLOR_RED);
    kprintf("\n*** PAGE FAULT ***\n");
    kprintf("Address: 0x%x\n", faulting_addr);
    kprintf("Error code: 0x%x\n", regs->err_code);

    if (!(regs->err_code & 0x1)) kprintf("  Page not present\n");
    if (regs->err_code & 0x2)    kprintf("  Write operation\n");
    if (regs->err_code & 0x4)    kprintf("  User mode\n");

    vga_set_color(COLOR_WHITE, COLOR_BLACK);
    while (1) { __asm__ __volatile__("hlt"); }
}

void paging_init(void) {
    /* Clear page directory */
    memset(page_directory, 0, sizeof(page_directory));

    /* Identity map first 4 MB (page table 0) */
    for (int i = 0; i < 1024; i++) {
        first_page_table[i] = (i * 0x1000) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    /* Identity map 4-8 MB */
    for (int i = 0; i < 1024; i++) {
        second_page_table[i] = ((1024 + i) * 0x1000) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    /* Identity map 8-12 MB */
    for (int i = 0; i < 1024; i++) {
        third_page_table[i] = ((2048 + i) * 0x1000) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    /* Identity map 12-16 MB */
    for (int i = 0; i < 1024; i++) {
        fourth_page_table[i] = ((3072 + i) * 0x1000) | PAGE_PRESENT | PAGE_WRITABLE;
    }

    /* Set page directory entries */
    page_directory[0] = ((uint32_t)first_page_table) | PAGE_PRESENT | PAGE_WRITABLE;
    page_directory[1] = ((uint32_t)second_page_table) | PAGE_PRESENT | PAGE_WRITABLE;
    page_directory[2] = ((uint32_t)third_page_table) | PAGE_PRESENT | PAGE_WRITABLE;
    page_directory[3] = ((uint32_t)fourth_page_table) | PAGE_PRESENT | PAGE_WRITABLE;

    /* Register page fault handler (INT 14) */
    register_interrupt_handler(14, page_fault_handler);

    /* Load page directory into CR3 and enable paging */
    __asm__ __volatile__(
        "mov %0, %%cr3\n"
        "mov %%cr0, %%eax\n"
        "or $0x80000000, %%eax\n"
        "mov %%eax, %%cr0\n"
        :
        : "r"(page_directory)
        : "eax"
    );
}
