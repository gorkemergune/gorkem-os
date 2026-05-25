/* GorkemOS - Kernel Heap Allocator */
#ifndef HEAP_H
#define HEAP_H

#include "types.h"

void  heap_init(void);
void *kmalloc(size_t size);
void  kfree(void *ptr);
uint32_t heap_get_used(void);

#endif
