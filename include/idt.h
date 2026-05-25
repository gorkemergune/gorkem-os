/* GorkemOS - Interrupt Descriptor Table */
#ifndef IDT_H
#define IDT_H

#include "types.h"

#define IDT_ENTRIES 256

/* IDT entry structure (8 bytes) */
struct idt_entry {
    uint16_t base_low;      /* Lower 16 bits of handler address */
    uint16_t selector;      /* Kernel code segment selector */
    uint8_t  always0;       /* Always 0 */
    uint8_t  flags;         /* Type and attributes */
    uint16_t base_high;     /* Upper 16 bits of handler address */
} __attribute__((packed));

/* IDT pointer structure (6 bytes) */
struct idt_ptr {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed));

/* Registers pushed by ISR stubs */
struct registers {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pusha */
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;              /* pushed by CPU */
};

/* ISR handler function type */
typedef void (*isr_handler_t)(struct registers *);

void idt_init(void);
void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags);
void register_interrupt_handler(uint8_t n, isr_handler_t handler);

#endif
