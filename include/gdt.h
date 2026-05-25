/* GorkemOS - Global Descriptor Table (C side) */
#ifndef GDT_H
#define GDT_H

#include "types.h"

/* GDT entry structure (8 bytes) */
struct gdt_entry {
    uint16_t limit_low;     /* Limit bits 0-15 */
    uint16_t base_low;      /* Base bits 0-15 */
    uint8_t  base_middle;   /* Base bits 16-23 */
    uint8_t  access;        /* Access byte */
    uint8_t  granularity;   /* Flags + Limit bits 16-19 */
    uint8_t  base_high;     /* Base bits 24-31 */
} __attribute__((packed));

/* GDT pointer structure (6 bytes) */
struct gdt_ptr {
    uint16_t limit;         /* Size of GDT - 1 */
    uint32_t base;          /* Address of first GDT entry */
} __attribute__((packed));

void gdt_init(void);
void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

#endif
