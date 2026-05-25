/* GorkemOS - I/O Port Access */
#ifndef IO_H
#define IO_H

#include "types.h"

/* Read a byte from an I/O port */
static inline uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ __volatile__("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

/* Write a byte to an I/O port */
static inline void outb(uint16_t port, uint8_t data) {
    __asm__ __volatile__("outb %0, %1" : : "a"(data), "Nd"(port));
}

/* Read a word (16-bit) from an I/O port */
static inline uint16_t inw(uint16_t port) {
    uint16_t result;
    __asm__ __volatile__("inw %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

/* Write a word (16-bit) to an I/O port */
static inline void outw(uint16_t port, uint16_t data) {
    __asm__ __volatile__("outw %0, %1" : : "a"(data), "Nd"(port));
}

/* Wait for an I/O operation to complete (small delay) */
static inline void io_wait(void) {
    outb(0x80, 0);
}

#endif
