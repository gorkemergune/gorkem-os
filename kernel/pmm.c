/* GorkemOS - Physical Memory Manager (Bitmap Allocator) */
#include "../include/pmm.h"
#include "../include/string.h"

/* Bitmap: each bit represents one 4KB page
 * 1 = used, 0 = free
 * Support up to 128 MB = 32768 pages = 4096 bytes bitmap */
#define MAX_PAGES 32768
static uint8_t bitmap[MAX_PAGES / PAGES_PER_BYTE];

static uint32_t total_pages = 0;
static uint32_t used_pages = 0;

static void bitmap_set(uint32_t page) {
    bitmap[page / 8] |= (1 << (page % 8));
}

static void bitmap_clear(uint32_t page) {
    bitmap[page / 8] &= ~(1 << (page % 8));
}

static int bitmap_test(uint32_t page) {
    return bitmap[page / 8] & (1 << (page % 8));
}

void pmm_init(uint32_t mem_size_kb) {
    total_pages = mem_size_kb / 4;   /* Each page is 4 KB */
    if (total_pages > MAX_PAGES) {
        total_pages = MAX_PAGES;
    }

    /* Mark all pages as used initially */
    memset(bitmap, 0xFF, sizeof(bitmap));
    used_pages = total_pages;

    /* Free usable memory (above 1 MB, skip first 1 MB for kernel/BIOS) */
    /* Pages from 256 (1 MB) to total_pages are usable */
    for (uint32_t i = 256; i < total_pages; i++) {
        bitmap_clear(i);
        used_pages--;
    }
}

void *pmm_alloc_page(void) {
    for (uint32_t i = 0; i < total_pages; i++) {
        if (!bitmap_test(i)) {
            bitmap_set(i);
            used_pages++;
            return (void *)(i * PAGE_SIZE);
        }
    }
    return NULL;  /* Out of memory */
}

void pmm_free_page(void *addr) {
    uint32_t page = (uint32_t)addr / PAGE_SIZE;
    if (page < total_pages && bitmap_test(page)) {
        bitmap_clear(page);
        used_pages--;
    }
}

uint32_t pmm_get_free_pages(void) {
    return total_pages - used_pages;
}

uint32_t pmm_get_total_pages(void) {
    return total_pages;
}
