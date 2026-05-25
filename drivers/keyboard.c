/* GorkemOS - Keyboard Driver (PS/2) */
#include "../include/keyboard.h"
#include "../include/idt.h"
#include "../include/isr.h"
#include "../include/io.h"
#include "../include/vga.h"

#define KB_DATA_PORT 0x60

/* Circular buffer for keyboard input */
static char kb_buffer[KB_BUFFER_SIZE];
static volatile int kb_read_idx = 0;
static volatile int kb_write_idx = 0;

/* US keyboard layout scancode -> ASCII mapping (lowercase) */
static const char scancode_to_ascii[128] = {
    0,   27, '1', '2', '3', '4', '5', '6', '7', '8',  /* 0-9 */
    '9', '0', '-', '=', '\b',                           /* 10-14 */
    '\t',                                                /* 15 - Tab */
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', /* 16-25 */
    '[', ']', '\n',                                      /* 26-28 */
    0,                                                   /* 29 - Left Ctrl */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l',      /* 30-38 */
    ';', '\'', '`',                                      /* 39-41 */
    0,                                                   /* 42 - Left Shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',           /* 43-50 */
    ',', '.', '/',                                       /* 51-53 */
    0,                                                   /* 54 - Right Shift */
    '*',                                                 /* 55 - Keypad * */
    0,                                                   /* 56 - Left Alt */
    ' ',                                                 /* 57 - Space */
    0,                                                   /* 58 - Caps Lock */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                      /* 59-68 - F1-F10 */
    0, 0,                                                /* 69-70 - Num Lock, Scroll Lock */
    0, 0, 0, '-',                                        /* 71-74 - Keypad */
    0, 0, 0, '+',                                        /* 75-78 - Keypad */
    0, 0, 0, 0, 0, 0, 0,                                /* 79-85 */
    0, 0, 0,                                             /* 86-88 */
    0, 0,                                                /* 89-90 - F11, F12 */
    0, 0, 0, 0, 0, 0, 0,                                /* 91-97 */
    0, 0, 0, 0, 0, 0, 0, 0, 0,                          /* 98-106 */
    0, 0, 0, 0, 0, 0, 0, 0, 0,                          /* 107-115 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0                 /* 116-127 */
};

/* Shifted keys */
static const char scancode_to_ascii_shift[128] = {
    0,   27, '!', '@', '#', '$', '%', '^', '&', '*',   /* 0-9 */
    '(', ')', '_', '+', '\b',                            /* 10-14 */
    '\t',                                                /* 15 */
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', /* 16-25 */
    '{', '}', '\n',                                      /* 26-28 */
    0,                                                   /* 29 */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',      /* 30-38 */
    ':', '"', '~',                                       /* 39-41 */
    0,                                                   /* 42 */
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M',            /* 43-50 */
    '<', '>', '?',                                       /* 51-53 */
    0, '*', 0, ' ', 0,                                   /* 54-58 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                      /* 59-68 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                      /* 69-78 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                      /* 79-88 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                      /* 89-98 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                      /* 99-108 */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,                      /* 109-118 */
    0, 0, 0, 0, 0, 0, 0, 0, 0                           /* 119-127 */
};

static volatile int shift_pressed = 0;

static void keyboard_callback(struct registers *regs) {
    (void)regs;

    uint8_t scancode = inb(KB_DATA_PORT);

    /* Key release (bit 7 set) */
    if (scancode & 0x80) {
        uint8_t released = scancode & 0x7F;
        if (released == 42 || released == 54) {  /* Left/Right Shift released */
            shift_pressed = 0;
        }
        return;
    }

    /* Shift press */
    if (scancode == 42 || scancode == 54) {
        shift_pressed = 1;
        return;
    }

    /* Convert scancode to ASCII */
    char c;
    if (shift_pressed) {
        c = scancode_to_ascii_shift[scancode];
    } else {
        c = scancode_to_ascii[scancode];
    }

    if (c != 0) {
        /* Add to buffer */
        int next = (kb_write_idx + 1) % KB_BUFFER_SIZE;
        if (next != kb_read_idx) {  /* Buffer not full */
            kb_buffer[kb_write_idx] = c;
            kb_write_idx = next;
        }
    }
}

void keyboard_init(void) {
    register_interrupt_handler(IRQ1, keyboard_callback);
}

int keyboard_has_input(void) {
    return kb_read_idx != kb_write_idx;
}

char keyboard_getchar(void) {
    /* Wait for input */
    while (!keyboard_has_input()) {
        __asm__ __volatile__("hlt");
    }

    char c = kb_buffer[kb_read_idx];
    kb_read_idx = (kb_read_idx + 1) % KB_BUFFER_SIZE;
    return c;
}
