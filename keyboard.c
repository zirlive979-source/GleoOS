#include <stdint.h>
#include "keyboard.h"

static const char scancode_set1[128] = {
    0, 27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0,'\\',
    'z','x','c','v','b','n','m',',','.','/', 0,'*',0,' ',
};

static uint8_t inb(uint16_t port) {
    uint8_t result;
    __asm__ volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

char keyboard_getchar(void) {
    while (1) {
        if (inb(0x64) & 1) {
            uint8_t code = inb(0x60);

            if (code & 0x80)
                continue;

            if (code < 128 && scancode_set1[code])
                return scancode_set1[code];
        }
    }
}
