/* GorkemOS - Print Utilities (kprintf) */
#include "../include/print.h"
#include "../include/vga.h"

/* va_list implementation for i686 */
typedef __builtin_va_list va_list;
#define va_start(ap, last) __builtin_va_start(ap, last)
#define va_arg(ap, type)   __builtin_va_arg(ap, type)
#define va_end(ap)         __builtin_va_end(ap)

void itoa(int value, char *buf, int base) {
    char *p = buf;
    char *p1, *p2;
    unsigned int uvalue;
    int negative = 0;

    if (base < 2 || base > 16) {
        *buf = '\0';
        return;
    }

    if (value < 0 && base == 10) {
        negative = 1;
        uvalue = (unsigned int)(-(value + 1)) + 1;
    } else {
        uvalue = (unsigned int)value;
    }

    /* Convert to string (reversed) */
    do {
        unsigned int digit = uvalue % base;
        *p++ = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        uvalue /= base;
    } while (uvalue > 0);

    if (negative) {
        *p++ = '-';
    }
    *p = '\0';

    /* Reverse the string */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void utoa(uint32_t value, char *buf, int base) {
    char *p = buf;
    char *p1, *p2;

    if (base < 2 || base > 16) {
        *buf = '\0';
        return;
    }

    do {
        unsigned int digit = value % base;
        *p++ = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        value /= base;
    } while (value > 0);

    *p = '\0';

    /* Reverse */
    p1 = buf;
    p2 = p - 1;
    while (p1 < p2) {
        char tmp = *p1;
        *p1 = *p2;
        *p2 = tmp;
        p1++;
        p2--;
    }
}

void kprintf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);

    char buf[32];

    for (int i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] != '%') {
            vga_putchar(fmt[i]);
            continue;
        }

        i++; /* Skip '%' */
        switch (fmt[i]) {
            case 'd': {
                int val = va_arg(args, int);
                itoa(val, buf, 10);
                vga_print(buf);
                break;
            }
            case 'u': {
                uint32_t val = va_arg(args, uint32_t);
                utoa(val, buf, 10);
                vga_print(buf);
                break;
            }
            case 'x': {
                uint32_t val = va_arg(args, uint32_t);
                utoa(val, buf, 16);
                vga_print(buf);
                break;
            }
            case 's': {
                const char *s = va_arg(args, const char *);
                if (s) {
                    vga_print(s);
                } else {
                    vga_print("(null)");
                }
                break;
            }
            case 'c': {
                char c = (char)va_arg(args, int);
                vga_putchar(c);
                break;
            }
            case '%':
                vga_putchar('%');
                break;
            default:
                vga_putchar('%');
                vga_putchar(fmt[i]);
                break;
        }
    }

    va_end(args);
}
