#include <stdint.h>
#include "keyboard.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((volatile uint16_t*)0xB8000)

static uint8_t color = 0x0F;
static uint8_t cursor_x = 0;
static uint8_t cursor_y = 0;

static void clear_screen(void) {
    for (uint32_t y = 0; y < VGA_HEIGHT; y++)
        for (uint32_t x = 0; x < VGA_WIDTH; x++)
            VGA_MEMORY[y * VGA_WIDTH + x] = ((uint16_t)color << 8) | ' ';
    cursor_x = 0;
    cursor_y = 0;
}

static void putc(char c) {
    if (c == '\n') {
        cursor_x = 0;
        cursor_y++;
    } else if (c == '\b') {
        if (cursor_x > 0) {
            cursor_x--;
            VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = ((uint16_t)color << 8) | ' ';
        }
    } else {
        VGA_MEMORY[cursor_y * VGA_WIDTH + cursor_x] = ((uint16_t)color << 8) | (uint8_t)c;
        cursor_x++;
        if (cursor_x >= VGA_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
    }

    if (cursor_y >= VGA_HEIGHT) {
        for (uint32_t y = 1; y < VGA_HEIGHT; y++)
            for (uint32_t x = 0; x < VGA_WIDTH; x++)
                VGA_MEMORY[(y - 1) * VGA_WIDTH + x] = VGA_MEMORY[y * VGA_WIDTH + x];

        for (uint32_t x = 0; x < VGA_WIDTH; x++)
            VGA_MEMORY[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = ((uint16_t)color << 8) | ' ';

        cursor_y = VGA_HEIGHT - 1;
    }
}

static void print(const char *s) {
    while (*s) putc(*s++);
}

static int streq(const char *a, const char *b) {
    while (*a && *b && *a == *b) { a++; b++; }
    return *a == '\0' && *b == '\0';
}

static void shell(void) {
    char command[64];
    uint32_t len = 0;

    print("GleoOS Terminal\n");
    print("Type 'help' to see commands.\n\n");

    while (1) {
        print("gleo> ");

        len = 0;
        while (1) {
            char c = keyboard_getchar();
            if (c == '\n') {
                command[len] = '\0';
                putc('\n');
                break;
            }
            if (c == '\b') {
                if (len > 0) {
                    len--;
                    putc('\b');
                }
                continue;
            }
            if (len < sizeof(command) - 1) {
                command[len++] = c;
                putc(c);
            }
        }

        if (streq(command, "help")) {
            print("Commands:\n");
            print("  help   - show commands\n");
            print("  clear  - clear screen\n");
            print("  about  - GleoOS information\n");
            print("  reboot - reboot the machine\n\n");
        } else if (streq(command, "clear")) {
            clear_screen();
        } else if (streq(command, "about")) {
            print("GleoOS v1.0.1\n");
            print("Developer: Dika\n");
            print("Educational mini-OS prototype.\n\n");
        } else if (streq(command, "reboot")) {
            print("Rebooting...\n");
            __asm__ volatile (
                "cli\n"
                "mov $0x64, %%dx\n"
                "mov $0xFE, %%al\n"
                "out %%al, %%dx\n"
                ::: "eax", "edx"
            );
        } else if (len != 0) {
            print("Unknown command. Type 'help'.\n\n");
        }
    }
}

void kmain(uint32_t magic, uint32_t multiboot_info) {
    (void)multiboot_info;
    clear_screen();

    print("========================================\n");
    print("              GleoOS 1.0.1\n");
    print("========================================\n");
    print("        Welcome to GleoOS!\n\n");

    if (magic != 0x2BADB002)
        print("Warning: unexpected Multiboot magic.\n");

    print("Boot successful. Kernel is running.\n\n");
    shell();

    for (;;) __asm__ volatile ("hlt");
}
