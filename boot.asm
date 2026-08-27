; GleoOS v1.0.1 - Multiboot entry
BITS 32

section .multiboot
align 4
dd 0x1BADB002              ; Multiboot magic
dd 0x00000003              ; flags: page align + memory info
dd -(0x1BADB002 + 0x00000003)

section .text
global _start
extern kmain

_start:
    cli
    mov esp, stack_top
    push ebx                    ; Multiboot information pointer
    push eax                    ; Multiboot magic
    call kmain

.hang:
    hlt
    jmp .hang

section .bss
align 16
stack_bottom:
    resb 16384
stack_top:
