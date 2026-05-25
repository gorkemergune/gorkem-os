/* GorkemOS - Paging */
#ifndef PAGING_H
#define PAGING_H

#include "types.h"

/* Page flags */
#define PAGE_PRESENT   0x1
#define PAGE_WRITABLE  0x2
#define PAGE_USER      0x4

void paging_init(void);

#endif
