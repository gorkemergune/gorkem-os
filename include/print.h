/* GorkemOS - Print Utilities */
#ifndef PRINT_H
#define PRINT_H

#include "types.h"

/* Convert integer to string */
void itoa(int value, char *buf, int base);
void utoa(uint32_t value, char *buf, int base);

/* Simple printf-like function */
/* Supports: %d, %u, %x, %s, %c, %% */
void kprintf(const char *fmt, ...);

#endif
