/* GorkemOS - Programmable Interval Timer (PIT) Driver */
#include "../include/timer.h"
#include "../include/idt.h"
#include "../include/isr.h"
#include "../include/io.h"

#define PIT_CHANNEL0  0x40
#define PIT_CMD       0x43
#define PIT_FREQUENCY 1193180   /* Base frequency of the PIT */

static volatile uint32_t tick_count = 0;

static void timer_callback(struct registers *regs) {
    (void)regs;  /* Unused */
    tick_count++;
}

uint32_t timer_get_ticks(void) {
    return tick_count;
}

void timer_init(uint32_t frequency) {
    /* Register the timer callback for IRQ0 (INT 32) */
    register_interrupt_handler(IRQ0, timer_callback);

    /* Calculate the divisor */
    uint16_t divisor = (uint16_t)(PIT_FREQUENCY / frequency);

    /* Send command byte: channel 0, access mode lo/hi, mode 3 (square wave) */
    outb(PIT_CMD, 0x36);

    /* Send divisor (low byte then high byte) */
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));
}
