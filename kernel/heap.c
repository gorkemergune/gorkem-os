/* GorkemOS - Kernel Heap Allocator (First-Fit) */
#include "../include/heap.h"
#include "../include/string.h"

/* Simple block header for tracking allocations */
struct heap_block {
    uint32_t size;               /* Size of this block (excluding header) */
    int      free;               /* 1 = free, 0 = allocated */
    struct heap_block *next;     /* Next block in the list */
};

#define HEAP_START 0x200000      /* Heap starts at 2 MB */
#define HEAP_SIZE  0x200000      /* 2 MB heap */
#define BLOCK_HEADER_SIZE sizeof(struct heap_block)
#define ALIGN(x) (((x) + 7) & ~7)  /* 8-byte alignment */

static struct heap_block *heap_head = NULL;
static uint32_t heap_used = 0;

void heap_init(void) {
    heap_head = (struct heap_block *)HEAP_START;
    heap_head->size = HEAP_SIZE - BLOCK_HEADER_SIZE;
    heap_head->free = 1;
    heap_head->next = NULL;
    heap_used = 0;
}

/* Split a block if it's too large */
static void split_block(struct heap_block *block, uint32_t size) {
    if (block->size >= size + BLOCK_HEADER_SIZE + 16) {
        struct heap_block *new_block =
            (struct heap_block *)((uint8_t *)block + BLOCK_HEADER_SIZE + size);
        new_block->size = block->size - size - BLOCK_HEADER_SIZE;
        new_block->free = 1;
        new_block->next = block->next;
        block->size = size;
        block->next = new_block;
    }
}

void *kmalloc(size_t size) {
    if (size == 0) return NULL;
    size = ALIGN(size);

    struct heap_block *block = heap_head;
    while (block) {
        if (block->free && block->size >= size) {
            split_block(block, size);
            block->free = 0;
            heap_used += block->size;
            return (void *)((uint8_t *)block + BLOCK_HEADER_SIZE);
        }
        block = block->next;
    }
    return NULL;  /* Out of memory */
}

/* Merge adjacent free blocks */
static void coalesce(void) {
    struct heap_block *block = heap_head;
    while (block && block->next) {
        if (block->free && block->next->free) {
            block->size += BLOCK_HEADER_SIZE + block->next->size;
            block->next = block->next->next;
        } else {
            block = block->next;
        }
    }
}

void kfree(void *ptr) {
    if (!ptr) return;

    struct heap_block *block =
        (struct heap_block *)((uint8_t *)ptr - BLOCK_HEADER_SIZE);
    block->free = 1;
    heap_used -= block->size;
    coalesce();
}

uint32_t heap_get_used(void) {
    return heap_used;
}
