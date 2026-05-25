/* GorkemOS - Interrupt Descriptor Table */
#include "../include/idt.h"
#include "../include/isr.h"
#include "../include/io.h"
#include "../include/string.h"
#include "../include/vga.h"
#include "../include/print.h"

static struct idt_entry idt[IDT_ENTRIES];
static struct idt_ptr   idtp;

/* Interrupt handlers registered by drivers */
static isr_handler_t interrupt_handlers[IDT_ENTRIES];

void idt_set_gate(uint8_t num, uint32_t base, uint16_t selector, uint8_t flags) {
    idt[num].base_low  = base & 0xFFFF;
    idt[num].base_high = (base >> 16) & 0xFFFF;
    idt[num].selector  = selector;
    idt[num].always0   = 0;
    idt[num].flags     = flags;
}

void register_interrupt_handler(uint8_t n, isr_handler_t handler) {
    interrupt_handlers[n] = handler;
}

/* PIC (Programmable Interrupt Controller) remapping */
static void pic_remap(void) {
    /* Save masks */
    uint8_t mask1 = inb(0x21);
    uint8_t mask2 = inb(0xA1);

    /* Start initialization sequence (ICW1) */
    outb(0x20, 0x11);    /* Master PIC */
    io_wait();
    outb(0xA0, 0x11);    /* Slave PIC */
    io_wait();

    /* Set vector offsets (ICW2) */
    outb(0x21, 0x20);    /* Master: IRQ 0-7  -> INT 32-39 */
    io_wait();
    outb(0xA1, 0x28);    /* Slave:  IRQ 8-15 -> INT 40-47 */
    io_wait();

    /* Tell Master about Slave at IRQ2 (ICW3) */
    outb(0x21, 0x04);
    io_wait();
    outb(0xA1, 0x02);
    io_wait();

    /* Set 8086 mode (ICW4) */
    outb(0x21, 0x01);
    io_wait();
    outb(0xA1, 0x01);
    io_wait();

    /* Restore masks */
    outb(0x21, mask1);
    outb(0xA1, mask2);
}

void idt_init(void) {
    idtp.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idtp.base  = (uint32_t)&idt;

    memset(&idt, 0, sizeof(struct idt_entry) * IDT_ENTRIES);
    memset(&interrupt_handlers, 0, sizeof(interrupt_handlers));

    /* Remap PIC: IRQ 0-15 -> INT 32-47 */
    pic_remap();

    /* Install CPU exception handlers (ISR 0-31) */
    /* 0x08 = kernel code segment, 0x8E = present, ring 0, 32-bit interrupt gate */
    idt_set_gate(0,  (uint32_t)isr0,  0x08, 0x8E);
    idt_set_gate(1,  (uint32_t)isr1,  0x08, 0x8E);
    idt_set_gate(2,  (uint32_t)isr2,  0x08, 0x8E);
    idt_set_gate(3,  (uint32_t)isr3,  0x08, 0x8E);
    idt_set_gate(4,  (uint32_t)isr4,  0x08, 0x8E);
    idt_set_gate(5,  (uint32_t)isr5,  0x08, 0x8E);
    idt_set_gate(6,  (uint32_t)isr6,  0x08, 0x8E);
    idt_set_gate(7,  (uint32_t)isr7,  0x08, 0x8E);
    idt_set_gate(8,  (uint32_t)isr8,  0x08, 0x8E);
    idt_set_gate(9,  (uint32_t)isr9,  0x08, 0x8E);
    idt_set_gate(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_gate(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_gate(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_gate(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_gate(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_gate(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_gate(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_gate(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_gate(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_gate(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_gate(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_gate(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_gate(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_gate(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_gate(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_gate(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_gate(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_gate(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_gate(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_gate(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_gate(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_gate(31, (uint32_t)isr31, 0x08, 0x8E);

    /* Install hardware IRQ handlers (32-47) */
    idt_set_gate(32, (uint32_t)irq0,  0x08, 0x8E);
    idt_set_gate(33, (uint32_t)irq1,  0x08, 0x8E);
    idt_set_gate(34, (uint32_t)irq2,  0x08, 0x8E);
    idt_set_gate(35, (uint32_t)irq3,  0x08, 0x8E);
    idt_set_gate(36, (uint32_t)irq4,  0x08, 0x8E);
    idt_set_gate(37, (uint32_t)irq5,  0x08, 0x8E);
    idt_set_gate(38, (uint32_t)irq6,  0x08, 0x8E);
    idt_set_gate(39, (uint32_t)irq7,  0x08, 0x8E);
    idt_set_gate(40, (uint32_t)irq8,  0x08, 0x8E);
    idt_set_gate(41, (uint32_t)irq9,  0x08, 0x8E);
    idt_set_gate(42, (uint32_t)irq10, 0x08, 0x8E);
    idt_set_gate(43, (uint32_t)irq11, 0x08, 0x8E);
    idt_set_gate(44, (uint32_t)irq12, 0x08, 0x8E);
    idt_set_gate(45, (uint32_t)irq13, 0x08, 0x8E);
    idt_set_gate(46, (uint32_t)irq14, 0x08, 0x8E);
    idt_set_gate(47, (uint32_t)irq15, 0x08, 0x8E);

    /* Load the IDT */
    __asm__ __volatile__("lidt (%0)" : : "r"(&idtp));

    /* Enable interrupts */
    __asm__ __volatile__("sti");
}

/* CPU Exception names for debugging */
static const char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 FPU Error",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point",
    "Virtualization Exception",
};

/* Called from isr_common_stub in assembly */
void isr_handler(struct registers *regs) {
    if (interrupt_handlers[regs->int_no]) {
        interrupt_handlers[regs->int_no](regs);
    } else if (regs->int_no < 32) {
        vga_set_color(COLOR_WHITE, COLOR_RED);
        vga_print("\n*** KERNEL PANIC ***\n");
        if (regs->int_no < 21) {
            kprintf("Exception: %s\n", exception_messages[regs->int_no]);
        }
        kprintf("INT: %d  ERR: %x  EIP: %x\n",
                regs->int_no, regs->err_code, regs->eip);
        vga_set_color(COLOR_WHITE, COLOR_BLACK);
        /* Halt */
        while (1) {
            __asm__ __volatile__("hlt");
        }
    }
}

/* Called from irq_common_stub in assembly */
void irq_handler(struct registers *regs) {
    /* Send EOI (End of Interrupt) to PIC */
    if (regs->int_no >= 40) {
        outb(0xA0, 0x20);   /* EOI to slave PIC */
    }
    outb(0x20, 0x20);       /* EOI to master PIC */

    /* Call registered handler if any */
    if (interrupt_handlers[regs->int_no]) {
        interrupt_handlers[regs->int_no](regs);
    }
}
