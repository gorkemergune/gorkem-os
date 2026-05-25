/* GorkemOS - Interactive Shell */
#include "../include/shell.h"
#include "../include/vga.h"
#include "../include/print.h"
#include "../include/keyboard.h"
#include "../include/string.h"
#include "../include/timer.h"
#include "../include/pmm.h"
#include "../include/heap.h"
#include "../include/io.h"

#define CMD_MAX_LEN 256
#define GORKEM_OS_VERSION "0.2"

static char cmd_buffer[CMD_MAX_LEN];
static int  cmd_len = 0;

static void print_prompt(void) {
    vga_print_color("gorkem", COLOR_LIGHT_GREEN, COLOR_BLACK);
    vga_print_color("> ", COLOR_LIGHT_CYAN, COLOR_BLACK);
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
}

/* ---- Shell Commands ---- */

static void cmd_help(void) {
    vga_set_color(COLOR_YELLOW, COLOR_BLACK);
    vga_print("Available commands:\n");
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
    vga_print("  help     - Show this help message\n");
    vga_print("  about    - About GorkemOS\n");
    vga_print("  clear    - Clear the screen\n");
    vga_print("  mem      - Show memory status\n");
    vga_print("  uptime   - Show system uptime\n");
    vga_print("  echo     - Echo text back\n");
    vga_print("  reboot   - Restart the system\n");
    vga_print("  shutdown - Halt the system\n");
}

static void cmd_about(void) {
    vga_set_color(COLOR_LIGHT_CYAN, COLOR_BLACK);
    vga_print("GorkemOS v");
    vga_print(GORKEM_OS_VERSION);
    vga_print("\n");
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
    vga_print("A hobby x86 operating system built from scratch.\n");
    vga_print("Written in C and Assembly by Gorkem.\n");
    vga_print("Architecture: x86 (i686), 32-bit Protected Mode\n");
}

static void cmd_clear(void) {
    vga_clear();
}

static void cmd_mem(void) {
    uint32_t free_pages = pmm_get_free_pages();
    uint32_t total_pages = pmm_get_total_pages();
    uint32_t used_pages = total_pages - free_pages;
    uint32_t heap_used_bytes = heap_get_used();

    vga_set_color(COLOR_YELLOW, COLOR_BLACK);
    vga_print("Memory Status:\n");
    vga_set_color(COLOR_WHITE, COLOR_BLACK);

    kprintf("  Physical pages: %u / %u used\n", used_pages, total_pages);
    kprintf("  Physical memory: %u KB / %u KB\n", used_pages * 4, total_pages * 4);
    kprintf("  Free memory: %u KB\n", free_pages * 4);
    kprintf("  Heap used: %u bytes\n", heap_used_bytes);

    /* Visual bar */
    uint32_t bar_len = 40;
    uint32_t filled = (used_pages * bar_len) / total_pages;
    vga_print("  [");
    vga_set_color(COLOR_LIGHT_GREEN, COLOR_BLACK);
    for (uint32_t i = 0; i < filled; i++) vga_putchar('#');
    vga_set_color(COLOR_DARK_GREY, COLOR_BLACK);
    for (uint32_t i = filled; i < bar_len; i++) vga_putchar('-');
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
    vga_print("]\n");
}

static void cmd_uptime(void) {
    uint32_t ticks = timer_get_ticks();
    uint32_t seconds = ticks / 100;
    uint32_t minutes = seconds / 60;
    seconds = seconds % 60;

    kprintf("Uptime: %u min %u sec (%u ticks)\n", minutes, seconds, ticks);
}

static void cmd_echo(const char *args) {
    if (args && *args) {
        vga_print(args);
        vga_print("\n");
    }
}

static void cmd_reboot(void) {
    vga_print("Rebooting...\n");
    /* Triple fault method: load empty IDT and trigger interrupt */
    uint8_t temp[6] = {0};
    __asm__ __volatile__("lidt (%0)" : : "r"(temp));
    __asm__ __volatile__("int $0x03");
}

static void cmd_shutdown(void) {
    vga_set_color(COLOR_YELLOW, COLOR_BLACK);
    vga_print("System halted. You can safely power off.\n");
    __asm__ __volatile__("cli; hlt");
}

/* ---- Command Processing ---- */

static void process_command(void) {
    /* Null-terminate */
    cmd_buffer[cmd_len] = '\0';

    /* Skip empty commands */
    if (cmd_len == 0) return;

    /* Find first space to separate command from args */
    char *args = NULL;
    for (int i = 0; i < cmd_len; i++) {
        if (cmd_buffer[i] == ' ') {
            cmd_buffer[i] = '\0';
            args = &cmd_buffer[i + 1];
            break;
        }
    }

    /* Match commands */
    if (strcmp(cmd_buffer, "help") == 0) {
        cmd_help();
    } else if (strcmp(cmd_buffer, "about") == 0) {
        cmd_about();
    } else if (strcmp(cmd_buffer, "clear") == 0) {
        cmd_clear();
    } else if (strcmp(cmd_buffer, "mem") == 0) {
        cmd_mem();
    } else if (strcmp(cmd_buffer, "uptime") == 0) {
        cmd_uptime();
    } else if (strcmp(cmd_buffer, "echo") == 0) {
        cmd_echo(args);
    } else if (strcmp(cmd_buffer, "reboot") == 0) {
        cmd_reboot();
    } else if (strcmp(cmd_buffer, "shutdown") == 0) {
        cmd_shutdown();
    } else {
        vga_print_color("Unknown command: ", COLOR_LIGHT_RED, COLOR_BLACK);
        vga_set_color(COLOR_WHITE, COLOR_BLACK);
        vga_print(cmd_buffer);
        vga_print("\nType 'help' for available commands.\n");
    }
}

void shell_init(void) {
    cmd_len = 0;
}

void shell_run(void) {
    vga_newline();
    vga_set_color(COLOR_YELLOW, COLOR_BLACK);
    vga_print("Shell ready. Type 'help' for commands.\n");
    vga_set_color(COLOR_WHITE, COLOR_BLACK);
    vga_newline();
    print_prompt();

    while (1) {
        char c = keyboard_getchar();

        if (c == '\n') {
            vga_newline();
            process_command();
            cmd_len = 0;
            print_prompt();
        } else if (c == '\b') {
            if (cmd_len > 0) {
                cmd_len--;
                vga_putchar('\b');
            }
        } else if (cmd_len < CMD_MAX_LEN - 1) {
            cmd_buffer[cmd_len++] = c;
            vga_putchar(c);
        }
    }
}
