/* GorkemOS - VGA Text Mode Driver */
#ifndef VGA_H
#define VGA_H

#include "types.h"

#define VGA_ADDRESS 0xB8000
#define VGA_WIDTH   80
#define VGA_HEIGHT  25

/* VGA color constants */
enum vga_color {
    COLOR_BLACK         = 0x0,
    COLOR_BLUE          = 0x1,
    COLOR_GREEN         = 0x2,
    COLOR_CYAN          = 0x3,
    COLOR_RED           = 0x4,
    COLOR_MAGENTA       = 0x5,
    COLOR_BROWN         = 0x6,
    COLOR_LIGHT_GREY    = 0x7,
    COLOR_DARK_GREY     = 0x8,
    COLOR_LIGHT_BLUE    = 0x9,
    COLOR_LIGHT_GREEN   = 0xA,
    COLOR_LIGHT_CYAN    = 0xB,
    COLOR_LIGHT_RED     = 0xC,
    COLOR_LIGHT_MAGENTA = 0xD,
    COLOR_YELLOW        = 0xE,
    COLOR_WHITE         = 0xF,
};

/* Create a VGA attribute byte from foreground and background colors */
#define VGA_COLOR(fg, bg) (((bg) << 4) | (fg))

void vga_init(void);
void vga_clear(void);
void vga_set_color(uint8_t fg, uint8_t bg);
void vga_putchar(char c);
void vga_print(const char *str);
void vga_print_at(const char *str, int row, int col);
void vga_print_color(const char *str, uint8_t fg, uint8_t bg);
void vga_newline(void);
void vga_scroll(void);

#endif
