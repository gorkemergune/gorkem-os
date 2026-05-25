/* GorkemOS - VGA Text Mode Driver */
#include "../include/vga.h"
#include "../include/string.h"

static volatile uint16_t *vga_buffer = (volatile uint16_t *)VGA_ADDRESS;
static int cursor_row = 0;
static int cursor_col = 0;
static uint8_t current_color = 0x0F; /* White on black */

/* Create a VGA entry (character + color) */
static uint16_t vga_entry(char c, uint8_t color) {
    return (uint16_t)c | ((uint16_t)color << 8);
}

void vga_init(void) {
    current_color = VGA_COLOR(COLOR_WHITE, COLOR_BLACK);
    vga_clear();
}

void vga_clear(void) {
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        vga_buffer[i] = vga_entry(' ', current_color);
    }
    cursor_row = 0;
    cursor_col = 0;
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    current_color = VGA_COLOR(fg, bg);
}

void vga_scroll(void) {
    /* Move all lines up by one */
    for (int i = 0; i < (VGA_HEIGHT - 1) * VGA_WIDTH; i++) {
        vga_buffer[i] = vga_buffer[i + VGA_WIDTH];
    }
    /* Clear the last line */
    for (int i = (VGA_HEIGHT - 1) * VGA_WIDTH; i < VGA_HEIGHT * VGA_WIDTH; i++) {
        vga_buffer[i] = vga_entry(' ', current_color);
    }
    cursor_row = VGA_HEIGHT - 1;
}

void vga_newline(void) {
    cursor_col = 0;
    cursor_row++;
    if (cursor_row >= VGA_HEIGHT) {
        vga_scroll();
    }
}

void vga_putchar(char c) {
    if (c == '\n') {
        vga_newline();
        return;
    }
    if (c == '\r') {
        cursor_col = 0;
        return;
    }
    if (c == '\t') {
        /* Tab: advance to next multiple of 4 */
        cursor_col = (cursor_col + 4) & ~3;
        if (cursor_col >= VGA_WIDTH) {
            vga_newline();
        }
        return;
    }
    if (c == '\b') {
        if (cursor_col > 0) {
            cursor_col--;
            int offset = cursor_row * VGA_WIDTH + cursor_col;
            vga_buffer[offset] = vga_entry(' ', current_color);
        }
        return;
    }

    int offset = cursor_row * VGA_WIDTH + cursor_col;
    vga_buffer[offset] = vga_entry(c, current_color);

    cursor_col++;
    if (cursor_col >= VGA_WIDTH) {
        vga_newline();
    }
}

void vga_print(const char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        vga_putchar(str[i]);
    }
}

void vga_print_at(const char *str, int row, int col) {
    int saved_row = cursor_row;
    int saved_col = cursor_col;
    cursor_row = row;
    cursor_col = col;
    vga_print(str);
    cursor_row = saved_row;
    cursor_col = saved_col;
}

void vga_print_color(const char *str, uint8_t fg, uint8_t bg) {
    uint8_t saved_color = current_color;
    vga_set_color(fg, bg);
    vga_print(str);
    current_color = saved_color;
}
